#pragma once

#include <string>
#include <ctime>

namespace MessageUtils {

    void copyStringToBuffer(const std::string& src,
                            char* dest,
                            size_t bufferSize);

    std::string bufferToString(const char* buffer,
                               size_t bufferSize);

    std::tm toLocalTime(std::time_t t);

    std::time_t parseTimestamp(const char* buffer);
    
    std::string timestampToStr(std::time_t t);
}

