#include "HEGLog.hpp"
#include <iostream>

namespace HEG {
void Logger::info(const char* msg) {
#if LOG_LEVEL > RELEASE
    std::cout << "HEG_INFO: " << msg << "\n";
#endif
}

void Logger::error(const char* msg) {
#if LOG_LEVEL > INFO
    std::cerr << "HEG_ERROR: " << msg << "\n";
#endif
}

} // namespace HEG