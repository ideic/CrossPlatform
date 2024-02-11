// RTPApp.cpp : Defines the entry point for the application.
//

#include <optional>
#include <string>
#include <iostream>
using namespace std;


int main()
{
	optional<string> test17{ " 17 Passed 2." };
    cout << "Hello CMake." << test17.value() << "\n";
	return 0;
}
