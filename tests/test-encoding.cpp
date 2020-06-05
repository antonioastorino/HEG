#include "BinaryMessageBuilder.hpp"
#include "HEGEncoding.hpp"
#include "HEGMap.hpp"
#include <assert.h>
#include <ostream>

int main() {
    std::cout << "-------- Huffman's encoding test. --------\n";

    // This is be done in preparation for the transmission and could be skipped once the encoding is
    // established and saved.
    std::cout << "Creating encoding based on the content of a file...\n";
    HEG::Encoding encoding("../assets/sample-text.txt");
    std::cout << "Done!\n";

    std::cout << "Printing encoded alphabet to file...\n";
    const char* encodingFileName = "../data/huffman-code.txt";
    encoding.printEncoding(encodingFileName);
    std::cout << "Done!\n";

    // The actual communication system starts here
    // --- Transmitter ---
    // The transmitter needs to load the alphabet and generate a map based on the encoding stored in
    // a file
    std::cout << "Loading encoded alphabet from file...\n";
    HEG::Map mp(encodingFileName);
    std::cout << "Done!\n";


    std::cout << "Converting message into bit stream...\n";
    std::string clearMsg   = "@&#^#$%^$%#&$Hello Huffman!!"; // Message to be transmitted
    std::string encodedMsg = ""; // Encoded message - not yet suitable for transmission
    for (size_t i = 0; i < clearMsg.size(); i++) {
        encodedMsg += mp.getCodeForSymbol(clearMsg[i]); // Huffman's encoding for 'b'
    }

    // convert string into binary sequence - message suitable for transmission

    std::vector<uint8_t> data = BinaryMessageBuilder::build<uint8_t>(encodedMsg);
    std::cout << "Done!\n";

    std::cout << "Decoing message from binary...\n";
    std::string receivedMsg = "";
    encoding.decode(data, 0, encodedMsg.size() - 1, receivedMsg);
    std::cout << "Done!\n";


    std::cout << "Message sent     :\t" << clearMsg << "\n";
    std::cout << "Message received :\t" << receivedMsg << "\n";
    assert(receivedMsg == clearMsg);

    // TODO: test that a given symbol's code is mapped correctly by looking into the tree of
    // `encoding` - use assert.
    return 0;
}