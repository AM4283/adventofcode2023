#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <map>
#include <regex>

std::vector<int> parseFile(std::ifstream* input) {
  std::vector<int> ret = {};
  std::string line;
  std::map<std::string,std::string> stringToNumber{
    {"one", "o1ne"},
    {"two", "2wo"},
    {"three", "3ree"},
    {"four", "4or"},
    {"five", "5ive"},
    {"six", "s6x"},
    {"seven", "s7even"},
    {"eight", "e8t"},
    {"nine", "n9ine"},
  };
  while (std::getline(*input, line)) {
    std::map<std::string,std::string>::iterator it;
    for(it = stringToNumber.begin(); it != stringToNumber.end(); ++it) {
      line = std::regex_replace(line, std::regex(it->first), it->second);
    }
    std::cout << "Newline: " << line << "\n";
    int firstDigit = 0;
    int lastDigit = 0;
    int i = 0;
    for (i=0; i < line.length(); i++) {
      if (std::isdigit(line[i])) {
        firstDigit = line[i] - '0';
        break;
      }
    }
    int j = 0;
    for (j = line.length()-1; j>=0; j--) {
      if (std::isdigit(line[j])) {
        lastDigit = line[j] - '0';
        break;
      }
    }
    ret.push_back(10*firstDigit + lastDigit);
  }
  return ret;
}

int main (int argc, char *argv[])
{
  if (argc != 2) {
    perror("not enough args");
    exit(1);
  }
  std::ifstream input (argv[1]);
  int sum = 0;
  if (input.is_open()) {
    std::vector<int> calibration_vals = parseFile(&input);
    for (auto i : calibration_vals) {
      std::cout << i << "\n";
    }
    sum = std::accumulate(calibration_vals.begin(), calibration_vals.end(), sum);
  }
  std::cout << "Sum of calibration vals " << sum << "\n";
  return 0;
}
