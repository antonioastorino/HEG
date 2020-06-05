

#include <iostream>
#include <utility>
#include <vector>
#include "HEGDataTypes.hpp"
#include "HEGEncoding.hpp"


int main() {
    std::cout << "Hello Huffman\n";

	HEG::Encoding encoding("../assets/sample-text.txt");

    auto alphabet = encoding.getAlphabet();

	encoding.printAlphabet();

	encoding.printEncoding("../data/huffman-code.txt");

    return 0;
}