#include "InputParser.h"
#include <algorithm>
#include <string>
#include <vector>

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
	auto found = std::ranges::find(_tokens, option);
	if (found != _tokens.end() && (*found != _tokens.back())) {
		return *found;
	}
	// NOLINTNEXTLINE(misc-include-cleaner)
	return ""s;
}

bool InputParser::cmdOptionExists(const std::string& option) const
{
	return std::ranges::contains(_tokens, option);
}
