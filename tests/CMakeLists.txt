
enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable(binaryop binaryop.cpp)
target_compile_options(binaryop PUBLIC -Wextra -Wpedantic -Weffc++)
target_compile_features(binaryop PUBLIC cxx_std_20)
target_link_libraries(binaryop  gtest_main)
add_test(BinaryOperators binaryop)

