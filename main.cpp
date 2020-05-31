
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

using freq_table = std::vector<std::pair<char, float>>;

struct HuffmanEncoding {
    long int index         = -1;
    float freq             = 0;
    HuffmanEncoding* left  = nullptr;
    HuffmanEncoding* right = nullptr;
};

void makeSymbolFrequencyTable(freq_table& alphabet) {
    std::ifstream file("assets/sample-text.txt");
    std::string s = "";
    char c;
    int numOfChars = 0;
    while (file.get(c)) {
        numOfChars++;
        s += c;
    };
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
        alphabet.push_back({c, static_cast<float>(freq) /* / static_cast<float>(numOfChars)*/});
        c    = s[i];
        freq = 0;
    }

    std::sort(alphabet.begin(), alphabet.end(),
              [](const std::pair<char, float>& a, const std::pair<char, float>& b) {
                  return a.second < b.second;
              });
}

void makeTree(const std::vector<HuffmanEncoding*>& nodes, HuffmanEncoding& tree) {
    if (nodes.size() == 2) {
        tree.right = nodes[0];
        tree.left  = nodes[1];
        tree.freq  = nodes[0]->freq + nodes[1]->freq;
    } else {
        HuffmanEncoding* node = new HuffmanEncoding();
        // always take a new entry initialize right leaf
        node->right = nodes[0];                        // connect to right child
        node->left  = nodes[1];                        // connect to left child
        node->freq  = nodes[0]->freq + nodes[1]->freq; // get frequency from the children
        std::vector<HuffmanEncoding*> newNodes;
        newNodes.push_back(node);
        for (size_t i = 2; i < nodes.size(); i++) { newNodes.push_back(nodes[i]); }
        std::sort(
            newNodes.begin(), newNodes.end(),
            [](const HuffmanEncoding* a, const HuffmanEncoding* b) { return a->freq < b->freq; });
        makeTree(newNodes, tree);
    }
}

void printTree(const HuffmanEncoding* tree, const freq_table& alphabet, std::string code = "") {
    if (tree->left == nullptr) {
        std::cout << code << " " << alphabet[tree->index].first << " \n";
        return;
    } else {
        printTree(tree->left, alphabet, code + "0");
    }
    if (tree->right == nullptr) {
        std::cout << " " << alphabet[tree->index].first << " \n";
        return;
    } else {
        printTree(tree->right, alphabet, code + "1");
    }
}

int main() {
    std::cout << "Hello Huffman\n";
    freq_table alphabet = {};

    makeSymbolFrequencyTable(alphabet);
    float totalP = 0;

    for (auto it = alphabet.begin(); it != alphabet.end(); it++) {
        totalP += it->second;
        std::cout << it->first << " " << it->second << "\n";
    }

    float maxP = 0;
    std::vector<HuffmanEncoding*> nodes;
    nodes.reserve(alphabet.size());
    for (size_t i = 0; i < alphabet.size(); i++) {
        HuffmanEncoding* node = new HuffmanEncoding();
        node->index           = i;
        node->freq            = alphabet[i].second;
        nodes.push_back(node);
    }

    HuffmanEncoding tree;
    makeTree(nodes, tree);

    printTree(&tree, alphabet);

    return 0;
}