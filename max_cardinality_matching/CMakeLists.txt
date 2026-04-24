# Max Cardinality Matching subdirectory
add_executable(matching_prototype max_cardinality_matching_prototype.cpp)
target_link_libraries(matching_prototype ${Boost_LIBRARIES})

add_executable(matching_test test_max_cardinality_matching.cpp)
target_link_libraries(matching_test ${Boost_LIBRARIES})

# Add tests
add_test(NAME matching_prototype_test COMMAND matching_prototype)
add_test(NAME matching_unit_test COMMAND matching_test)