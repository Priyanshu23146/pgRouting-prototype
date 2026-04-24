# run_tests.cmake - Custom test runner for pgRouting prototypes

# Function to run a test and report result
function(run_test test_name test_command)
    message(STATUS "Running ${test_name}...")
    execute_process(
        COMMAND ${test_command}
        RESULT_VARIABLE test_result
        OUTPUT_VARIABLE test_output
        ERROR_VARIABLE test_error
    )

    if(test_result EQUAL 0)
        message(STATUS "${test_name} PASSED")
        if(test_output)
            message("${test_output}")
        endif()
    else()
        message(STATUS "${test_name} FAILED")
        if(test_error)
            message("Error: ${test_error}")
        endif()
        if(test_output)
            message("Output: ${test_output}")
        endif()
    endif()
    message("")
endfunction()

# Run all prototype tests
message(STATUS "=== Running pgRouting Prototype Tests ===")

# Test max cardinality matching
run_test("Max Cardinality Matching Prototype" "${CMAKE_BINARY_DIR}/max_cardinality_matching/matching_prototype")
run_test("Max Cardinality Matching Unit Tests" "${CMAKE_BINARY_DIR}/max_cardinality_matching/matching_test")

# Test biconnected components
run_test("Biconnected Components Prototype" "${CMAKE_BINARY_DIR}/biconnected_components/biconnected_prototype")
run_test("Biconnected Components Unit Tests" "${CMAKE_BINARY_DIR}/biconnected_components/biconnected_test")

message(STATUS "=== Test Summary ===")
message(STATUS "All tests completed. Check individual results above.")