// RTPApp.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <optional>
using namespace std;
//#include <unistd.h>
//#include <limits.h>


int main()
{
	//char hostname[HOST_NAME_MAX];
	//char username[LOGIN_NAME_MAX];
	//gethostname(hostname, HOST_NAME_MAX);
	//getlogin_r(username, LOGIN_NAME_MAX);
	
	//cout << "Hello CMake."<< hostname << " uname:"<< username << endl;
	optional<string> test17{ " 17 Passed 2." };
	cout << "Hello CMake." << test17.value() << endl;
	//std::cin.get();
	return 0;
}
