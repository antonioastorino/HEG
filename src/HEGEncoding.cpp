#include "HEGEncoding.hpp"
#include "HEGDataTypes.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

HEG::Encoding::Encoding(const char* filename) : alphabet_({}), tree_(), leaves_({}), nodes_({}) {
    // create a list of symbols and corresponding frequencies
    this->makeSymbolFrequencyTable(filename);
    // create a leave in for the tree for each symbol

    leaves_.reserve(alphabet_.size());
    for (size_t i = 0; i < alphabet_.size(); i++) {
        HuffmanEncoding* leave = new HuffmanEncoding();
        leave->index           = i;
        leave->freq            = alphabet_[i].second;
        leaves_.push_back(leave);
    }
    this->makeTree(leaves_);
}

std::string HEG::Encoding::charToString(char c) {
        std::stringstream s;
        if (c < 33 || c > 126) {
            s << "ch(" <<  static_cast<int>(c) << ")";
        } else {
            s << c;
        }
		return s.str();
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
    uint32_t freq = 0;
    // populate the alphabet with symbol and correspondng frequency
    while (s[i] != '\0') {
        while (s[i] == c) {
            freq++;
            i++;
        }
        alphabet_.push_back({c, static_cast<float>(freq) /* / static_cast<float>(numOfChars)*/});
        c    = s[i];
        freq = 0;
    }

    std::sort(alphabet_.begin(), alphabet_.end(),
              [](const symbol_t& a, const symbol_t& b) { return a.second < b.second; });
}

void HEG::Encoding::makeTree(const std::vector<HuffmanEncoding*>& tmp_nodes) {
    if (tmp_nodes.size() == 2) {
        tree_.right = tmp_nodes[0];
        tree_.left  = tmp_nodes[1];
        tree_.freq  = tmp_nodes[0]->freq + tmp_nodes[1]->freq;
    } else {
        // add a node to the tree
        HuffmanEncoding* node = new HuffmanEncoding();
        nodes_.push_back(node);

        // connect the new node
        node->right = tmp_nodes[0];                            // connect to right child
        node->left  = tmp_nodes[1];                            // connect to left child
        node->freq  = tmp_nodes[0]->freq + tmp_nodes[1]->freq; // get frequency from the children

        // create a vector of nodes for the new makeTree() call
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

void HEG::Encoding::printEncoding() { this->printTree(&tree_, ""); }

void HEG::Encoding::printAlphabet() {
    std::cout << "Symbol\t|  Frequency\n";
    std::cout << "--------+-----------\n";
    for (auto it = alphabet_.begin(); it != alphabet_.end(); it++) {
        std::cout << charToString(it->first) << "\t|  " << it->second << "\n";
    }
    std::cout << "--------+-----------\n";
}

void HEG::Encoding::printTree(const HuffmanEncoding* tree, std::string code) {
    if (tree->left == nullptr) {
        std::cout << code << " -> " << charToString(alphabet_[tree->index].first) << " \n";
        return;
    } else {
        printTree(tree->left, code + "0");
    }
    if (tree->right == nullptr) {
        std::cout << " " << charToString(alphabet_[tree->index].first) << " \n";
        return;
    } else {
        printTree(tree->right, code + "1");
    }
}