# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
# if (POLICY CMP0141)
#  cmake_policy(SET CMP0141 NEW)
#  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
#endif()]

project ("RTPApp")

# Include sub-projects.
add_subdirectory ("RTPApp")

if (CMAKE_BUILD_TYPE STREQUAL Release)
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif(MSVC)
	  add_compile_options(PRIVATE /W4 /WX)
else()
	  add_compile_options(PRIVATE -Wall -Wextra -Wpedantic -Werror -g)
endif()

