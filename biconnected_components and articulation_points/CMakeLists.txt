# Biconnected Components subdirectory
add_executable(biconnected_prototype "biconnected_components and articulation_points.cpp")
target_link_libraries(biconnected_prototype ${Boost_LIBRARIES})

add_executable(biconnected_test test_biconnected_components.cpp)
target_link_libraries(biconnected_test ${Boost_LIBRARIES})

# Add tests
add_test(NAME biconnected_prototype_test COMMAND biconnected_prototype)
add_test(NAME biconnected_unit_test COMMAND biconnected_test)