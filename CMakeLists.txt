cmake_minimum_required(VERSION 3.0)

project(FlatMap
    VERSION 0.0.1
    LANGUAGES CXX
)

option(FLATMAP_BUILD_BENCMARKS "Build map benchmarks" ON)
option(FLATMAP_BUILD_TESTS     "Build unittests"      ON)

add_library(WarningFlags INTERFACE)
target_compile_options(WarningFlags INTERFACE
    -Wall
    -Werror
)

# Create 'linkable' target, to use in your project do:
# target_link_libraries(<target> PUBLIC FlatMap)
add_library(FlatMap INTERFACE)
target_include_directories(FlatMap INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_include_directories(FlatMap SYSTEM INTERFACE
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)
install(DIRECTORY include/FlatMap DESTINATION include)
# target_include_directories(FlatMap INTERFACE include/)
# target_include_directories(FlatMap
#     INTERFACE
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
#     $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
# )

# namespaced alias:
add_library(FlatMap::FlatMap ALIAS FlatMap)

add_subdirectory(third_party)
add_subdirectory(include)

if (FLATMAP_BUILD_TESTS)
    add_subdirectory(test)
endif (FLATMAP_BUILD_TESTS)

if (FLATMAP_BUILD_BENCMARKS)
    add_subdirectory(bench)
endif (FLATMAP_BUILD_BENCMARKS)
