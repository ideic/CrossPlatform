﻿# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.25)

# Enable Hot Reload for MSVC compilers if supported.
# if (POLICY CMP0141)
#  cmake_policy(SET CMP0141 NEW)
#  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
#endif()]

project ("RTPApp")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#if (MSVC)
#	add_compile_options(/Qspectre)
#endif()
if (CMAKE_BUILD_TYPE STREQUAL Release)
	if (MSVC)
		add_compile_options(/O2)
	else()
		add_compile_options(-O3)
	endif()
endif()
if(MSVC)
	  add_compile_options(/W4 /WX)
else()
	  add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Werror -Wformat=2 -Wunused -Wundef -g)
endif()

if (MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#else ()
#	set(CMAKE_CXX_FLAGS_RELEASE "-MT")
#	set(CMAKE_CXX_FLAGS_DEBUG "-MTd")
endif()

option(ASAN_ENABLED "Build this target with AddressSanitizer" OFF)

if(ASAN_ENABLED)
   if(MSVC)
	 if (CMAKE_BUILD_TYPE STREQUAL Debug)
		add_compile_options(/fsanitize=address)
	 endif()
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
unset(ASAN_ENABLED CACHE)

# Include sub-projects.
add_subdirectory ("RTPApp")
add_subdirectory ("Server")
add_subdirectory ("Client")
add_subdirectory ("Network")
add_subdirectory ("Common")

add_subdirectory ("CommonTest")
