#pragma once
#include <string>
#include <vector>
class InputParser {
	std::vector <std::string> _tokens;
public:
	InputParser(int argc, char** argv);
	std::string getCmdOption(const std::string& option) const;
	bool cmdOptionExists(const std::string& option) const;
};
