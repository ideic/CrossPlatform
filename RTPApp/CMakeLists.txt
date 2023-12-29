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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_BUILD_TYPE STREQUAL Release)
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif(MSVC)
	  add_compile_options(/W4 /WX)
else()
	  add_compile_options(-Wall -Wextra -Wpedantic -Werror -g)
endif()

option(ASAN_ENABLED "Build this target with AddressSanitizer" ON)

if(ASAN_ENABLED)
   if(MSVC)
     add_compile_options(/fsanitize=address)
   elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
     add_compile_options(-fsanitize=undefined,thread)
     add_link_options(-fsanitize=undefined,thread)
   elseif(CMAKE_BUILD_TYPE STREQUAL Debug) # Clang doesn't support ASAN in debug mode
	 message(WARNING "ASAN is not supported in debug mode")
   else()
	 add_compile_options(-fsanitize=address,undefined)
	 add_link_options(-fsanitize=address,undefined)
   endif()
endif()

# Include sub-projects.
add_subdirectory ("RTPApp")
add_subdirectory ("Server")
add_subdirectory ("Client")
add_subdirectory ("Network")
add_subdirectory ("common")

