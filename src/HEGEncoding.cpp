#include "HEGEncoding.hpp"
#include "HEGDataTypes.hpp"
#include "HEGLog.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <wchar.h>
#ifdef __linux__
#include <algorithm>
#endif

HEG::Encoding::Encoding(const char* filename) : alphabet_({}), tree_(), leaves_({}), nodes_({}) {
    // create a list of symbols and corresponding frequencies
    this->makeSymbolFrequencyTable(filename);

    // create one leave in the tree for each symbol
    leaves_.reserve(alphabet_.size());
    for (size_t i = 0; i < alphabet_.size(); i++) {
        HuffmanEncoding* leave = new HuffmanEncoding();
        leave->index           = i;
        leave->freq            = alphabet_[i].second;
        leaves_.push_back(leave);
    }
    this->makeTree(leaves_);
    this->p_currNode_ = &this->tree_;
}

HEG::Encoding::~Encoding() {
    // delete all leaves
    for (size_t i = 0; i < leaves_.size(); i++) {
        delete leaves_[i];
        leaves_[i] = nullptr;
    }
    // delete all nodes
    for (size_t i = 0; i < nodes_.size(); i++) {
        delete nodes_[i];
        nodes_[i] = nullptr;
    }
}

alphabet_t HEG::Encoding::getAlphabet() const { return alphabet_; }

void HEG::Encoding::makeSymbolFrequencyTable(const char* filename) {
    std::string s = "";
    char c;
    int numOfChars = 0;

    {
        // open file for reading
        std::ifstream file(filename);
        // Measure the length of the file and copy to string
        while (file.get(c)) {
            numOfChars++;
            s += c;
        };
        file.close();
    }

    // sort
    std::sort(s.begin(), s.begin() + numOfChars);

    int i         = 0;
    c             = s[i];
    uint64_t freq = 0;
    // populate the alphabet with symbol and correspondng frequency
    while (s[i] != '\0') {
        while (s[i] == c) {
            freq++;
            i++;
        }
        alphabet_.push_back({c, freq});
        c    = s[i];
        freq = 0;
    }

    alphabet_.push_back({'\0', -1}); // add char `0` with highest possible frequency to access it
                                     // quickly as a message terminator

    // sort by frequency as needed by Huffman's algorithm
    std::sort(alphabet_.begin(), alphabet_.end(),
              [](const symbol_t& a, const symbol_t& b) { return a.second < b.second; });
}

void HEG::Encoding::makeTree(const std::vector<HuffmanEncoding*>& tmp_nodes) {
    // Recursively, pairs of nodes are connected to a parent from the leaves to the head. If only
    // two nodes don't have a parent, the parent must be the root of the tree.
    if (tmp_nodes.size() == 2) {
        tree_.right = tmp_nodes[0];
        tree_.left  = tmp_nodes[1];
        tree_.freq  = tmp_nodes[0]->freq + tmp_nodes[1]->freq;
    } else {
        // Add a parent node to the tree and assign as children the nodes with highest frequency.
        // Because the nodes are sorted at each iteration, the highest frequencies are at the first
        // and the second node in the vector.
        HuffmanEncoding* node = new HuffmanEncoding();
        nodes_.push_back(node);

        // connect the new node
        node->right = tmp_nodes[0];                            // connect to right child
        node->left  = tmp_nodes[1];                            // connect to left child
        node->freq  = tmp_nodes[0]->freq + tmp_nodes[1]->freq; // get frequency from the children

        // Create a vector of nodes for the new makeTree() call. The vector contains all the nodes
        // that don't have a parent yet, namely those from the 3rd on.
        std::vector<HuffmanEncoding*> newNodes;
        newNodes.push_back(node);
        for (size_t i = 2; i < tmp_nodes.size(); i++) { newNodes.push_back(tmp_nodes[i]); }
        std::sort(
            newNodes.begin(), newNodes.end(),
            [](const HuffmanEncoding* a, const HuffmanEncoding* b) { return a->freq < b->freq; });

        // continue recursively
        makeTree(newNodes);
    }
}

void HEG::Encoding::printEncoding(const char* outFileName) const {
    // Inspect all the nodes using DFS and print the encoding by associating a '0' to a inspecting a
    // left child and a '1' otherwise
    // If an output file name is not provided, the encoding will be printed to the stardad output,
    // otherwise to the specified file
    if (outFileName[0] == 0) {
        this->printTree(&tree_, "", std::cout);
    } else {
        std::ofstream file(outFileName);
        this->printTree(&tree_, "", file);
        file.close();
    }
}

void HEG::Encoding::printAlphabet() const {
    // The list of symbols and corresponding frequencies is printed using wprintf to display
    // extended ASCII chars
    std::cout << "Symbol\t|  Frequency\n";
    std::cout << "--------+-----------\n";
    setlocale(LC_CTYPE, "UTF-8");
    for (auto it = alphabet_.begin(); it != alphabet_.end(); it++) {
        if (it->first < 33 || it->first == 127) {
            std::cout << "ch(" << static_cast<int>(it->first) << ")"
                      << "\t|  " << it->second << "\n";

        } else {
            wprintf(L"%lc\t|  %lu\n", static_cast<wchar_t>(it->first), it->second);
        }
    }
    std::cout << "--------+-----------\n";
}

void HEG::Encoding::printTree(const HuffmanEncoding* tree, std::string code,
                              std::ostream& outstream) const {
    if (tree->left == nullptr) {
        outstream << code << " " << static_cast<int>(alphabet_.at(tree->index).first) << "\n";
        return;
    } else {
        printTree(tree->left, code + "0", outstream);
    }
    // NOTE: no need to check if right == nullptr because it would be only when
    // left = nullptr, already checked above
    printTree(tree->right, code + "1", outstream);
}

template <typename T>
void HEG::Encoding::decode(const std::vector<T>& data, const size_t startBit, const size_t endBit,
                           std::string& out_msg) {
    bool currBitValue;
    if (p_currNode_->left == nullptr) HEG::Logger::error("Empty encoding!");
    constexpr uint8_t bytesPerElement = sizeof(data[0]);
    constexpr uint8_t bitsPerElement  = bytesPerElement * 8; // num of bytes per element * 8
    const uint64_t totalNumOfBits     = bitsPerElement * data.size();

    for (size_t currBit = startBit; currBit < totalNumOfBits && currBit <= endBit; currBit++) {
        uint64_t currElement = currBit / bitsPerElement;
        uint8_t scanner      = currBit % bitsPerElement;
        currBitValue         = (data.at(currElement) >> scanner) & 1;

        if (currBitValue == false) {
            p_currNode_ = p_currNode_->left;
            HEG::Logger::debug("Got 0, going left");
        } else {
            p_currNode_ = p_currNode_->right;
            HEG::Logger::debug("Got 1, going right");
        }

        if (p_currNode_->index != -1) { // a leave is reached
            uint8_t symbol = this->alphabet_.at(p_currNode_->index).first;
            if (symbol == '\0') {
                this->p_currNode_ = &this->tree_;
                return;
            }
            out_msg += symbol; // add the corresponding symble to out_msg
            std::stringstream s;
            s << "Leave with symbol " << symbol << " reached";
            HEG::Logger::info(s.str().c_str());
            this->p_currNode_ = &this->tree_; // reset p_currNode_ to the root of the tree
        }
        // TODO: reset and exit if null is transmitted
    }
}

template void HEG::Encoding::decode(const std::vector<uint8_t>&, size_t, size_t, std::string&);
template void HEG::Encoding::decode(const std::vector<uint16_t>&, size_t, size_t, std::string&);
template void HEG::Encoding::decode(const std::vector<uint32_t>&, size_t, size_t, std::string&);
template void HEG::Encoding::decode(const std::vector<uint64_t>&, size_t, size_t, std::string&);