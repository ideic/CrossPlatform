# CMakeList.txt : CMake project for RTPApp, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Server "RTPServer.cpp" )

target_link_libraries(Server PUBLIC Common)
target_include_directories(Server PUBLIC "${PROJECT_SOURCE_DIR}")