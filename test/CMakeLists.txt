add_executable(unittest
    detail/catch_main.cpp
    test_static_flat_map.cpp
    test_flat_map.cpp
    )
set_target_properties(unittest PROPERTIES CXX_STANDARD 17)
target_link_libraries(unittest PUBLIC WarningFlags)
target_link_libraries(unittest PUBLIC Catch2)
target_link_libraries(unittest PUBLIC FlatMap)
