#ifndef HEGEncoding_hpp
#define HEGEncoding_hpp
#include <vector>

namespace HEG {

class Encoding {
private:
    Encoding();
    std::vector<std::pair<char, float>> alphabet;

public:
    Encoding(const char*);
    void makeSymbolFrequencyTable(const char* filename);
    std::vector<std::pair<char, float>> getAlphabet() const;
};

} // namespace HEG

#endif /* HEGEncoding_hpp */