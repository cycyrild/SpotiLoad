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
    -ffunction-sections
    -fdata-sections
    -fno-unwind-tables
    -fno-asynchronous-unwind-tables
    -fno-ident
    -Wall
    -Wextra
    -Wpedantic
    -fno-stack-protector
    -fno-stack-check
    -mno-stack-arg-probe
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
)

add_link_options(
    -fuse-ld=lld
    -nostdlib
    -nodefaultlibs
    -nostartfiles
    -fno-rtlib-defaultlib
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ALIAS_STANDARD_FUNCTION_NAMES HARD CACHE STRING "" FORCE)

add_subdirectory(external/printf)
add_subdirectory(external/minhook)
add_subdirectory(external/etl)


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
        ${PROJECT_SOURCE_DIR}/include/runtime

    PRIVATE
        ${PROJECT_SOURCE_DIR}/include/sp_load
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    WIN32_LEAN_AND_MEAN
    PRINTF_ALIAS_STANDARD_FUNCTION_NAMES_HARD=1
)

target_link_options(${PROJECT_NAME} PRIVATE
    -Wl,-e,DllMain
    -Wl,--exclude-all-symbols
    -Wl,-Map,${PROJECT_NAME}.map
    -Wl,--gc-sections
    -Wl,--no-insert-timestamp
    -Wl,--strip-all
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        printf::printf
        etl::etl
        minhook
)
