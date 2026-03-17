cmake_minimum_required(VERSION 3.10)
project(dotnet-native-runtime LANGUAGES C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_REQUIRED_FLAGS -std=gnu23)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_compile_definitions(_GNU_SOURCE)
add_compile_options(
    -g
    -ffreestanding
    -fno-stack-protector
    -maccumulate-outgoing-args
    -fno-stack-check
    -fno-pic
    -fno-pie
    -finline
    -fshort-wchar
    -masm=intel
    -mno-red-zone
    -msse4.2
    -mno-avx
    -Wall
    -Wextra
)

add_subdirectory("${PROJECT_SOURCE_DIR}/src/math")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/SystemNative")
# add_subdirectory("${PROJECT_SOURCE_DIR}/src/SystemGlobalizationNative")
# add_subdirectory("${PROJECT_SOURCE_DIR}/src/Rh")
