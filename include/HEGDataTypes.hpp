#ifndef HEGDataTypes_hpp
#define HEGDataTypes_hpp

struct HuffmanEncoding {
    long int index         = -1;
    float freq             = 0;
    HuffmanEncoding* left  = nullptr;
    HuffmanEncoding* right = nullptr;
};

#endif /* HEGDataTypes_hpp */