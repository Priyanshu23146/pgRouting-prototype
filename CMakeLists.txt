cmake_minimum_required(VERSION 3.15)
project(pgRouting-prototype VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find Boost
find_package(Boost 1.65 REQUIRED COMPONENTS graph)

# Include directories
include_directories(${Boost_INCLUDE_DIRS})

# Enable testing
enable_testing()

# Add subdirectories
add_subdirectory(max_cardinality_matching)
add_subdirectory(biconnected_components)

# Custom target to build all prototypes
add_custom_target(prototypes)
add_dependencies(prototypes matching_prototype biconnected_prototype)

# Custom target to build all tests
add_custom_target(tests)
add_dependencies(tests matching_test biconnected_test)