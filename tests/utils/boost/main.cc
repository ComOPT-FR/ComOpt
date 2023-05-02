#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <vector>

int main() {
  std::string input = "boost library test";
  std::vector<std::string> result;
  boost::split(result, input, boost::is_any_of(" "));

  for (const auto& str : result) {
    std::cout << str << std::endl;
  }

  return 0;
}