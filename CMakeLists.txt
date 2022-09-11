cmake_minimum_required(VERSION 3.0.0)
project(CTDT VERSION 0.1.0)

include(CTest)
enable_testing()



add_library(CTDT ctdt.hpp)
target_compile_options(CTDT PRIVATE -O3 -Wextra -Wpedantic -Weffc++ -Wshadow)
target_compile_features(CTDT PRIVATE cxx_std_20)
set_target_properties(CTDT PROPERTIES LINKER_LANGUAGE CXX)



# TESTS 

add_subdirectory(tests)





set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
