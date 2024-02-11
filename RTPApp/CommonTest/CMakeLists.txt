 include(FetchContent)
 FetchContent_Declare(
   googletest
   URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
 )

# For Windows: Prevent overriding the parent project's compiler/linker settings
 set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
 FetchContent_MakeAvailable(googletest)

 # Silence clang-tidy warnings from googletest
 set_target_properties(gtest PROPERTIES CXX_CLANG_TIDY "")
 set_target_properties(gtest_main PROPERTIES CXX_CLANG_TIDY "")
 set_target_properties(gmock PROPERTIES CXX_CLANG_TIDY "")
 set_target_properties(gmock_main PROPERTIES CXX_CLANG_TIDY "")

enable_testing()
add_executable(CommonTest  "BlockingQueueTest.cpp" "ThreadManagerTest.cpp")
#set_property(TARGET CommonTest PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(CommonTest  GTest::gtest_main Common)

include(GoogleTest)
gtest_discover_tests(CommonTest)

target_include_directories(CommonTest PUBLIC "${PROJECT_SOURCE_DIR}")