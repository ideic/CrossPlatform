#include "InputParser.h"
#include <algorithm>
using namespace std::string_literals;
InputParser::InputParser(int argc, char** argv)
{
  for (int i = 1; i < argc; ++i) { 
	  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	  _tokens.emplace_back(argv[i]); 
  }
}

std::string InputParser::getCmdOption(const std::string& option) const
{
	auto found = std::find(_tokens.begin(), _tokens.end(), option);
	if (found != _tokens.end() && ++found != _tokens.end()) {
		return *found;
	}
	return ""s;
}

bool InputParser::cmdOptionExists(const std::string& option) const
{
	return std::find(_tokens.begin(), _tokens.end(), option) != _tokens.end();
}
