# CMakeList.txt : CMake project for RTPApp, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library(common STATIC InputParser.cpp "InputParser.h" "ThreadManager.h" "ThreadManager.cpp" "BlockingQueue.h" "BlockingQueue.cpp" "Logger/Logger.h") 
