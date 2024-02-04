# CMakeList.txt : CMake project for RTPApp, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library(Common STATIC InputParser.cpp "InputParser.h" "ThreadManager.h" "ThreadManager.cpp" "BlockingQueue.h" "BlockingQueue.cpp" "Logger/Logger.h") 
#target_include_directories(Common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )