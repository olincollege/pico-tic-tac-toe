cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(TicTacToe C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()
set(PICO_BOARD pico_w)


add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

# Include the source files.
add_subdirectory(src)

# Include bluetooth
include_directories(/usr/include/bluetooth)

# Include keypad library
add_subdirectory(keypad_lib)

set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wconversion")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wdouble-promotion")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wduplicated-branches")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wduplicated-cond")
# The following line is only needed in LLVM/clang
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wimplicit-fallthrough")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wlogical-op")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmisleading-indentation")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wnull-dereference")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshadow")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wsign-conversion")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunreachable-code")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

# No options are set here because the configuration is set in .clang-tidy
# instead. If that file is not present, clang-tidy will be of limited help.
set(CMAKE_C_CLANG_TIDY "clang-tidy")

# In current courses that teach C, we use the Criterion testing framework. If
# If you want to use a different framework, or if you want to skip testing
# entirely (not recommended), change the lines below.
# include(CTest)
# find_library(CRITERION criterion REQUIRED)
# set(CRITERION_FLAGS --verbose -j1)

# add_subdirectory(test)
