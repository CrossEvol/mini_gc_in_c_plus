cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_GENERATOR "MinGW Makefiles")

project(mini_gc)

set(CMAKE_CXX_STANDARD 17)

# Include FetchContent
include(FetchContent)

# Declare fmt
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.2.1
)

# Declare Catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)

# Make the libraries available
FetchContent_MakeAvailable(fmt Catch2)

############################################################
# Create a library
############################################################

add_library(gc_lib STATIC
        src/object.cpp
        src/vm.cpp
)
add_library(gc::library ALIAS gc_lib)

target_include_directories(gc_lib
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(gc_lib PRIVATE fmt::fmt)

############################################################
# Create Main executable
############################################################

add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Link libraries to main executable
target_link_libraries(${PROJECT_NAME} PRIVATE gc::library)

############################################################
# Create Test executable
############################################################

add_executable(unit_tests
    test/unit_tests.cpp
)

# Link libraries to test executable
target_link_libraries(unit_tests PRIVATE 
    gc::library
    Catch2::Catch2WithMain
)
