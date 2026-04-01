# pgRouting Prototype for Algorithm

## Overview

This repository contains standalone C++ prototypes of Boost Graph Library (BGL) algorithms intended for future integration into pgRouting.

The goal of these prototypes is to demonstrate algorithm correctness, practical use in transportation networks, and compatibility with the pgRouting architecture (SQL → C bridge → C++ driver → BGL).

The implementations focus on graph matching and network robustness analysis, extending pgRouting beyond traditional shortest-path algorithms.

---

## Algorithms Implemented in this prototype

### 1. pgr_maxCardinalityMatching

Uses `boost::edmonds_maximum_cardinality_matching()` to compute the maximum matching in an undirected graph.

This algorithm finds the largest set of edges such that no two edges share a common vertex.

**Use cases:**

- Ride-sharing pairing
- Delivery assignment optimization
- Resource allocation in transportation networks

---

### 2. pgr_biconnected_components

Uses `boost::biconnected_components()` and `boost::articulation_points()` to analyze the biconnectivity of the graph.

Biconnected components are the maximal sets of edges that remain connected after removing any single vertex. Articulation points are vertices whose removal increases the number of connected components.

This algorithm identifies the structural integrity and weak points in transportation networks.

**Use cases:**

- Identifying critical junctions (articulation points)
- Finding bridge edges
- Network robustness and vulnerability analysis
- Disaster planning and evacuation routing

---

## Requirements

- C++17 compiler (GCC 8+, Clang 6+)
- CMake ≥ 3.15
- Boost ≥ 1.65

Install Boost:

- Ubuntu: sudo apt install libboost-all-dev
- macOS: brew install boost
- windows: choco install boost-msvc-14.3

---

### Build Instruction

From the project root (pgRouting-prototypes/):

```bash
mkdir build && cd build
cmake ..
make
make test
```

Optional: if Boost is not in a standard location, set `BOOST_ROOT`:

```bash
cmake .. -DBOOST_ROOT=/path/to/boost
make
make test

```

To build a single target (eg. max_Cardinality_Matching Prototype) :

```bash
make matching_prototype
```

Standalone compile (without CMake) for one file, from the algorithm directory:

```bash
g++ -std=c++17 -Wall -Wextra matching_prototype.cpp -o matching_prototype -lboost_graph
g++ -std=c++17 -Wall -Wextra biconnected_components.cpp -o biconnected_components.cpp -lboost_graph
```

---

### Running tests

Run all test executables and get PASS/FAIL per test:

```bash
cd build
make test
```

Or run each test binary manually:

```bash
./matching_prototype
./biconnected_components_prototype
```

---

### Sample output

matching_prototype

```bash
=== Path Graph ===
Vertices V = 6, Edges E = 5

Matching pairs:
0 <--> 1
2 <--> 3
4 <--> 5

Matching size = 3
Unmatched vertices = 0

```

biconnected_components_prototype

```bash
=== Biconnected Components Demo ===
Vertices V = 5, Edges E = 5

=== Graph Edges ===
0 - 1
1 - 2
0 - 3
1 - 3
2 - 4

Edge component assignments:
0 - 1 (component=2)
1 - 2 (component=1)
0 - 3 (component=2)
1 - 3 (component=2)
2 - 4 (component=0)

Articulation points: 1, 2
Biconnected components count = 3
```

Actual command used:

```bash
./biconnected_components_prototype
```

---

### Implementation status

- `max_cardinality_matching_prototype.cpp`: implemented and tested in repository.
- `biconnected_components and articulation_points/biconnected_components and articulation_points.cpp`: placeholder file exists; implementation logic should use `boost::biconnected_components` and `boost::articulation_points`.

---

### How results map to pgRouting functions (SQL signature)

1. pgr_maxCardinalityMatching (postgreSQL wrapper)

- SQL signature (example):
  `pgr_maxCardinalityMatching(edges_sql_table, start_vid, end_vid, directed := false)`
- Internals:
  - SQL wrapper validates inputs and builds edge list
  - C bridge converts rows to C/C++ graph structure
  - C++ driver constructs a `boost::adjacency_list` graph
  - BGL call: `boost::edmonds_maximum_cardinality_matching(g, &mate[0])`
- Output mapping:
  - matched pairs returned as set of edge IDs or vertex pairs
  - matching size returned separately

2. biconnected_components (postgreSQL wrapper)

- SQL signature (example):
  `pgr_biconnectedComponents(edges_sql_table, start_vid, end_vid, directed := false)`
- Internals:
  - SQL wrapper validates inputs and builds edge list
  - C bridge constructs C++ graph type
  - C++ driver invokes BGL:
    - `boost::biconnected_components(g, component)`
    - `boost::articulation_points(g, art_points)`
- Output mapping:
  - each edge annotated with component ID
  - articulation points returned as vertex list

---

Each algorithm follows the standard pgRouting architecture:

| Layer       | Role                                                                    |
| ----------- | ----------------------------------------------------------------------- |
| SQL wrapper | User calls `pgr_maxCardinalityMatching()` or `biconnected_components()` |
| C bridge    | Handles PostgreSQL input/output and calls C++ driver                    |
| C++ driver  | Builds graph and executes BGL algorithm                                 |
| BGL call    | Executes core algorithm                                                 |

These prototypes directly use Boost Graph Library functions, ensuring smooth transition into pgRouting with minimal changes.

---

### Goal

To extend pgRouting with:

- Graph matching capabilities
- Network robustness analysis tools
- Advanced transportation network insights

These features support:

- Logistics optimization
- Traffic management
- Infrastructure planning
- Disaster response analysis

---

## Project structure

```bash
pgRouting-prototype
│
├── .vscode/
│   └── c_cpp_properties.json
│
├── max_cardinality_matching/
│   ├── max_cardinality_matching_prototype.cpp
│   └── test_max_cardinality_matching.cpp
│
├── biconnected_components and articulation_points/
│   ├── biconnected_components_prototype.cpp
│   └── test_biconnected_components.cpp
│
├── CMakeLists.txt
├── run_tests.cmake
└── README.md
```
