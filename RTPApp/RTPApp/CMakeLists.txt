# CMakeList.txt : CMake project for RTPApp, include source and define
# project specific logic here.
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Add source to this project's executable.
add_executable (RTPApp "RTPApp.cpp" "RTPApp.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RTPApp PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
option(ASAN_ENABLED "Build this target with AddressSanitizer" ON)

if(ASAN_ENABLED)
   if(MSVC)
     add_compile_options(PUBLIC /fsanitize=address)
   else()
     add_compile_options(<target> PUBLIC -fsanitize=address <additional-options>)
     add_link_options(<target> PUBLIC -fsanitize=address)
   endif()
endif()