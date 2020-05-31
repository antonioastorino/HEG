#include "HEGEncoding.hpp"
#include "HEGDataTypes.hpp"
#include <fstream>

HEG::Encoding::Encoding(const char* filename) { this->makeSymbolFrequencyTable(filename); }

std::vector<std::pair<char, float>> HEG::Encoding::getAlphabet() const { return this->alphabet; }

void HEG::Encoding::makeSymbolFrequencyTable(const char* filename) {
    std::ifstream file(filename);
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
        this->alphabet.push_back(
            {c, static_cast<float>(freq) /* / static_cast<float>(numOfChars)*/});
        c    = s[i];
        freq = 0;
    }

    std::sort(this->alphabet.begin(), alphabet.end(),
              [](const std::pair<char, float>& a, const std::pair<char, float>& b) {
                  return a.second < b.second;
              });
}