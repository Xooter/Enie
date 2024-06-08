cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Enie VERSION 1.0)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(Enie src/main.cpp ${SRC_FILES})
target_include_directories(Enie PUBLIC src/${CMAKE_CURRENT_SOURCE_DIR} )

if(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(Enie PRIVATE -Os -s)
    set_target_properties(Enie PROPERTIES LINK_FLAGS_RELEASE "-s")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
