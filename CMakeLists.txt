cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Windows)

set(CMAKE_C_COMPILER clang-20)
set(CMAKE_CXX_COMPILER clang++-20)
set(CMAKE_ASM_COMPILER clang-20)

set(CMAKE_C_COMPILER_TARGET x86_64-w64-windows-gnu)
set(CMAKE_CXX_COMPILER_TARGET x86_64-w64-windows-gnu)
set(CMAKE_ASM_COMPILER_TARGET x86_64-w64-windows-gnu)

project(
    sp_load
    VERSION 0.1.0
    LANGUAGES C CXX ASM
)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_compile_options(
    -march=skylake
    -mtune=skylake
    -ffunction-sections
    -fdata-sections
    -fno-exceptions
    -fno-unwind-tables
    -fno-asynchronous-unwind-tables
    -fno-ident
    -Wall
    -Wextra
    -Wpedantic
    -fno-stack-protector
    -fno-stack-check
    -mno-stack-arg-probe
    -fno-threadsafe-statics
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
)

add_link_options(
    -fuse-ld=lld
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ALIAS_STANDARD_FUNCTION_NAMES HARD CACHE STRING "" FORCE)

add_subdirectory(external/printf)
add_subdirectory(external/minhook)

add_library(boost INTERFACE)

file(GLOB BOOST_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/external/boost/libs/*/include
)

target_include_directories(boost SYSTEM INTERFACE
    ${PROJECT_SOURCE_DIR}/external/boost
    ${BOOST_INCLUDE_DIRS}
)

target_compile_definitions(boost INTERFACE
    BOOST_NO_EXCEPTIONS
    BOOST_CONTAINER_NO_LIB
)

target_compile_options(printf PRIVATE -Wno-everything)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    src/*.cc
    src/*.S
)

add_library(${PROJECT_NAME} SHARED
    ${SRC_FILES}
)

target_include_directories(${PROJECT_NAME}
    BEFORE PRIVATE
        ${PROJECT_SOURCE_DIR}/include/std

    PRIVATE
        ${PROJECT_SOURCE_DIR}/include/sp_load
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    WIN32_LEAN_AND_MEAN
    PRINTF_ALIAS_STANDARD_FUNCTION_NAMES_HARD=1
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -fno-builtin-printf
)

target_link_options(${PROJECT_NAME} PRIVATE
    -nostdlib
    -Wl,-e,DllMain
    -Wl,--exclude-all-symbols
    -Wl,-Map,${PROJECT_NAME}.map
    -Wl,--gc-sections
    -Wl,--no-insert-timestamp
    -Wl,--strip-all
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    printf::printf
    minhook
    kernel32
    boost
)
