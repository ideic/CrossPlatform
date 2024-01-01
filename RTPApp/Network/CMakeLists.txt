# CMakeList.txt : CMake project for RTPApp, include source and define
# project specific logic here.
#

# Add source to this project's executable.

add_library(Network STATIC TCPClient.cpp "UDPClient.h" "UDPClient.cpp" "TCPClient.h" "UDPServer.h" "UDPServer.cpp" "NetworkHeader.h") 
target_include_directories(Network PUBLIC "${PROJECT_SOURCE_DIR}")