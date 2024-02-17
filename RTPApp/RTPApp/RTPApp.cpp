// RTPApp.cpp : Defines the entry point for the application.
//

#include <optional>
#include <string>
#include <iostream>
#include <exception>

using namespace std;

// NOLINTNEXTLINE(bugprone-exception-escape)
int main()
{
	try {
		optional<string> test17{ " 17 Passed 2." };
		cout << "Hello CMake." << test17.value() << "\n";
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
	catch (...) {
		std::cerr << "Unknown exception" << '\n';
	}
	return 0;
}
