#include "HEGMap.hpp"
#include "HEGEncoding.hpp"
#include <ostream>

int main() {
    std::cout << "Hello test!\n";

	HEG::Encoding encoding("assets/sample-text.txt");
	
	const char* encodingFileName = "data/huffman-code.txt";
	encoding.printEncoding(encodingFileName);

	HEG::Map mp(encodingFileName);

	std::cout << "Code for symbol 4 = " << mp.getCodeForSymbol('4') << "\n";

    return 0;
}