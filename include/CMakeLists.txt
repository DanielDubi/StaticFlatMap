# add_library(FlatMap INTERFACE)
target_sources(FlatMap INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/FlatMap/StaticFlatMap.hpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/flatmaps/flat_map.hpp"
    )
# target_include_directories(FlatMap INTERFACE
#     "${CMAKE_CURRENT_SOURCE_DIR}")
