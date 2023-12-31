// RTPApp.cpp : Defines the entry point for the application.
//

#include "RTPServer.h"
#include "../Common/InputParser.h"
using namespace std;
//#include <unistd.h>
//#include <limits.h>

void Usage()
{
	cout << "Usage: RTPServer --port <port-number>\n";
}
int main(int argc, char** argv)
{	cout << "Hello\n" ;
	InputParser input(argc, argv);
	string port = input.getCmdOption("--port");

	if (port.empty())
	{
		Usage();
		return 2;
	}


	//std::cin.get();
	return 0;
}
