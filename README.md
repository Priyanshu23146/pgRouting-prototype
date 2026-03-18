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

```bash
Ubantu: sudo apt install libboost-all-dev
macOS: brew install boost
windows: choco install boost-msvc-14.3

===

### Build Instruction

```

From the project root (pgRouting-prototypes/):

mkdir build && cd build
cmake ..
make
