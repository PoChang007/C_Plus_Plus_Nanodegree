#include <string>

#include "format.h"

using std::string;

void stringConversion(std::string &inputString, long inputUnit)
{
  if (inputUnit < 10) {
    inputString = "0" + std::to_string(inputUnit);
  } else {
    inputString = std::to_string(inputUnit);
  }
}

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  long s = seconds % 60;
  long m = (seconds / 60) % 60;
  long h = (seconds / (60 * 60)) % 24;

  std::string ss = "";
  std::string ms = "";
  std::string hs = "";

  stringConversion(ss, s);
  stringConversion(ms, m);
  stringConversion(hs, h);

  return string(hs + ":" + ms + ":" + ss);
}