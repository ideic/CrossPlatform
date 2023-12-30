// RTPApp.cpp : Defines the entry point for the application.
//

#include "RTPApp.h"
#include <optional>
#include <string>
using namespace std;


int main()
{
	optional<string> test17{ " 17 Passed 2." };
	cout << "Hello CMake." << test17.value() << endl;
	return 0;
}
