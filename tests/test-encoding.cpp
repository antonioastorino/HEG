#include "HEGEncoding.hpp"
#include "HEGMap.hpp"
#include <ostream>
#include <assert.h>


int main() {
    std::cout << "Hello test!\n";

    HEG::Encoding encoding("../assets/sample-text.txt");

    const char* encodingFileName = "../data/huffman-code.txt";
    encoding.printEncoding(encodingFileName);

    HEG::Map mp(encodingFileName);

    std::cout << "Code for symbol 'b' = " << mp.getCodeForSymbol('b') << "\n";
    std::cout << "Code for symbol 'f' = " << mp.getCodeForSymbol('f') << "\n";

    std::vector<uint8_t> data = {0b00000000, 0b00011000};
    std::string out = "";
    encoding.decode(data, 0, 16, out);

    // TODO: test that a given symbol's code is mapped correctly by looking into the tree of
    // `encoding` - use assert.
    return 0;
}