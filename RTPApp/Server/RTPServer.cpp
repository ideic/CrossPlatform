// RTPApp.cpp : Defines the entry point for the application.
//
#include "Common/InputParser.h"
#include <iostream>
using namespace std;

void Usage()
{
	cout << "Usage: RTPServer --port <port-number>\n";
}
int main(int argc, char** argv)
{	
	try {
        cout << "Hello\n";
        const InputParser input(argc, argv);
        const string port = input.getCmdOption("--port");

        if (port.empty()) {
          Usage();
          return 2;
        }  
	} 
    catch (const std::exception &e) {
        cerr<< e.what() << endl;
          return -1;
	}

	//std::cin.get();
	return 0;
}
