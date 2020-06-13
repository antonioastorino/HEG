#ifndef HEGMap_hpp
#define HEGMap_hpp
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

/**
 * @class Map
 * Reads the file containing the encoding and creates and unordered map in which the key is an
 * alphabet element and the value is the corresponding code (a string of zeros and ones)
 */
namespace HEG {
class Map {
private:
    Map();
    std::unordered_map<char, std::string> mp_;

public:
    Map(const char*);
    std::string getCodeForSymbol(char);
};
} // namespace HEG

#endif /* HEGMap_hpp */
