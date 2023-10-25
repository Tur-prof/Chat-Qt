#ifndef PARSING_H
#define PARSING_H

#include <string>

class Parsing{
  public:
    Parsing() = delete;

    static uint8_t getMinLength();

    static bool isTooShort(const std::string& input);
    static bool isContainsForbidenChars(const std::string& input);
    static bool isStartsByNumber(const std::string& input);
};

#endif // PARSING_H
