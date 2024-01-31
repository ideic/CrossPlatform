 include(FetchContent)
 FetchContent_Declare(
   googletest
   URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
 )

# For Windows: Prevent overriding the parent project's compiler/linker settings
 set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
 FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(NetworkTest  "UDPServerTest.cpp")
#set_property(TARGET CommonTest PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(NetworkTest  GTest::gtest_main Common Network)
if (WIN32)
	target_link_libraries(NetworkTest  wsock32 ws2_32)
endif()
include(GoogleTest)
gtest_discover_tests(NetworkTest)

target_include_directories(NetworkTest PUBLIC "${PROJECT_SOURCE_DIR}")

