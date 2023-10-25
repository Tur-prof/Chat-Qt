#include "Parsing.h"

namespace {
  const uint8_t MIN_LENGTH_USER_INPUT = 3;
}

uint8_t Parsing::getMinLength()
{
  return MIN_LENGTH_USER_INPUT;
}

bool Parsing::isTooShort(const std::string& input)
{
  if (input.length() < MIN_LENGTH_USER_INPUT){
    return true;
  }
  return false;
}

bool Parsing::isContainsForbidenChars(const std::string& input)
{
  const std::string permissionedChars =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  size_t pos = input.find_first_not_of(permissionedChars);
  if (pos != std::string::npos) {
    return true;
  }
  return false;
}

bool Parsing::isStartsByNumber(const std::string& input)
{
  const std::string numbers = "0123456789";
  size_t pos = input.find_first_of(numbers);
  if (pos == 0) {
    return true;
  }
  return false;
}
