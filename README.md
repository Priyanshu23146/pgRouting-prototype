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

### 2. pgr_edgeConnectivity

Uses `boost::edge_connectivity()` to compute the minimum number of edges whose removal disconnects the graph.

This measures the robustness of a network.

**Use cases:**

- Identifying critical road segments
- Infrastructure resilience analysis
- Disaster planning and evacuation routing

---

## Requirements

- C++17 compiler (GCC 8+, Clang 6+)
- CMake ≥ 3.15
- Boost ≥ 1.65

Install Boost:

- Ubantu: sudo apt install libboost-all-dev
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
g++ -std=c++17 -Wall -Wextra edge_connectivity.cpp -o edge_connectivity_prototype -lboost_graph
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
./edge_connectivity_prototype
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

edge_connectivity_prototype

```bash
=== Road Network Graph ===
Vertices V = 5, Edges E = 6

Edge Connectivity: 2

Interpretation:
At least 2 roads must be removed to disconnect the network.

```

---

### pgRouting Integration Notes

---

Each algorithm follows the standard pgRouting architecture:

| Layer       | Role                                                                  |
| ----------- | --------------------------------------------------------------------- |
| SQL wrapper | User calls `pgr_maxCardinalityMatching()` or `pgr_edgeConnectivity()` |
| C bridge    | Handles PostgreSQL input/output and calls C++ driver                  |
| C++ driver  | Builds graph and executes BGL algorithm                               |
| BGL call    | Executes core algorithm                                               |

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
pgrouting-prototypes-priyanshu
│
├── .github/workflows/
│   └── ci.yml
│
├── max_cardinality_matching/
│   ├── max_cardinality_matching_prototype.cpp
│   └── test_max_cardinality_matching.cpp
│
├── edge_connectivity/
│   ├── edge_connectivity_prototype.cpp
│   └── test_edge_connectivity.cpp
│
├── CMakeLists.txt
├── run_tests.cmake
└── README.md
```
