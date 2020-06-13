#include "HEGMap.hpp"
#include <sstream>

HEG::Map::Map(const char* filename) {
    std::string s = "";
    char c;
    int numOfChars = 0;

    // open file for reading
    std::ifstream file(filename);
    // convert each line read into a map element
    std::string line;
    while (std::getline(file, line)) {
        std::string code;
        int symbol;
        std::istringstream iss(line);
        iss >> code >> symbol;
        this->mp_[static_cast<uint8_t>(symbol)] = code;
    }
    file.close();
}

std::string HEG::Map::getCodeForSymbol(char symbol) { return this->mp_[symbol]; }
