// RTPApp.cpp : Defines the entry point for the application.
//
#include "Common/InputParser.h"
#include <iostream>
#include <string>
#include <exception>
using namespace std;
namespace
{
  void Usage() { cout << "Usage: RTPServer --port <port-number>\n"; }
}

// NOLINTNEXTLINE(bugprone-exception-escape)
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
        cerr<< e.what() << '\n';
        return -1;
	}

	//std::cin.get();
	return 0;
}
