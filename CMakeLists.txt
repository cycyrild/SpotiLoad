cmake_minimum_required(VERSION 3.20)

project(
    sp_load
    VERSION 0.1.0
    LANGUAGES C CXX ASM
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/nt-crtless-runtime)
add_subdirectory(external/minhook)
add_subdirectory(external/etl)

target_compile_definitions(minhook PRIVATE MINHOOK_DISABLE_INTRINSICS)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    src/*.cc
    src/*.S
)

add_library(${PROJECT_NAME} SHARED
    ${SRC_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include/sp_load
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        WIN32_LEAN_AND_MEAN
        _CRT_USE_BUILTIN_OFFSETOF=0
)

target_link_options(${PROJECT_NAME} PRIVATE
    "LINKER:/entry:DllMain"
    "LINKER:/map:$<TARGET_FILE_BASE_NAME:${PROJECT_NAME}>.map"
    "LINKER:/opt:ref"
    "LINKER:/opt:icf"
    "LINKER:/timestamp:0"
    "LINKER:/lldsavetemps"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        nt-crtless-runtime
        etl::etl
        minhook
)
