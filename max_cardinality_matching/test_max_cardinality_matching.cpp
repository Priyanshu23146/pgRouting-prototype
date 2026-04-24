// Compile:
// g++ -std=c++17 -Wall -Wextra test_max_cardinality_matching.cpp -o test_max_cardinality_matching -lboost_graph
//
// Test file for max cardinality matching prototype
// Tests various graph structures and validates matching results

#include <iostream>
#include <vector>
#include <cassert>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using Graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS>;

using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

// ------------------------------------------------------------
// Utility: Print Graph Edges
// ------------------------------------------------------------
void print_graph(const Graph& g) {
    std::cout << "=== Graph Edges ===\n";

    for (auto edge : boost::make_iterator_range(edges(g))) {
        std::cout << source(edge, g) << " - " << target(edge, g) << "\n";
    }
    std::cout << "\n";
}

// ------------------------------------------------------------
// Utility: Print Matching Result
// ------------------------------------------------------------
void print_matching(const Graph& g, const std::vector<Vertex>& mate) {
    std::cout << "Matching pairs:\n";

    for (size_t i = 0; i < mate.size(); ++i) {
        if (mate[i] != boost::graph_traits<Graph>::null_vertex() && i < mate[i]) {
            std::cout << i << " <--> " << mate[i] << "\n";
        }
    }
    std::cout << "\n";
}

// ------------------------------------------------------------
// Analysis: Matching statistics
// ------------------------------------------------------------
void analyze_matching(const Graph& g, const std::vector<Vertex>& mate) {
    int V = boost::num_vertices(g);
    int matching_size = boost::matching_size(g, &mate[0]);

    std::cout << "Matching size = " << matching_size << "\n";
    std::cout << "Unmatched vertices = " << (V - 2 * matching_size) << "\n";

    // Theoretical check
    std::cout << "Check: matching_size <= V/2 = "
              << matching_size << " <= " << V / 2 << "\n\n";
}

// ------------------------------------------------------------
// Test Function: Run Matching Algorithm and Validate
// ------------------------------------------------------------
bool test_matching(Graph& g, const std::string& name, int expected_matching_size) {
    int V = boost::num_vertices(g);
    int E = boost::num_edges(g);

    std::cout << "========================================\n";
    std::cout << "Testing: " << name << "\n";
    std::cout << "Vertices V = " << V << ", Edges E = " << E << "\n\n";

    print_graph(g);

    std::vector<Vertex> mate(V);

    // Core algorithm
    boost::edmonds_maximum_cardinality_matching(g, &mate[0]);

    // Validation
    bool valid = boost::checked_edmonds_maximum_cardinality_matching(g, &mate[0]);

    print_matching(g, mate);

    std::cout << "Valid matching: " << (valid ? "YES" : "NO") << "\n";

    analyze_matching(g, mate);

    int actual_matching_size = boost::matching_size(g, &mate[0]);

    if (actual_matching_size == expected_matching_size && valid) {
        std::cout << "TEST PASSED: Matching size matches expected value\n\n";
        return true;
    } else {
        std::cout << "TEST FAILED: Expected matching size " << expected_matching_size
                  << ", got " << actual_matching_size << "\n\n";
        return false;
    }
}

// ------------------------------------------------------------
// Graph Builders for Testing
// ------------------------------------------------------------

// Empty graph
Graph build_empty_graph() {
    return Graph(0);
}

// Single vertex
Graph build_single_vertex_graph() {
    return Graph(1);
}

// Two vertices, no edge
Graph build_two_vertices_no_edge() {
    return Graph(2);
}

// Two vertices with edge
Graph build_two_vertices_with_edge() {
    Graph g(2);
    add_edge(0, 1, g);
    return g;
}

// Path graph P6
Graph build_path_graph() {
    Graph g(6);
    add_edge(0,1,g);
    add_edge(1,2,g);
    add_edge(2,3,g);
    add_edge(3,4,g);
    add_edge(4,5,g);
    return g;
}

// Cycle graph C6
Graph build_cycle_graph() {
    Graph g(6);
    for (int i = 0; i < 6; ++i)
        add_edge(i, (i + 1) % 6, g);
    return g;
}

// Complete graph K4
Graph build_complete_graph_k4() {
    Graph g(4);
    for (int i = 0; i < 4; ++i)
        for (int j = i + 1; j < 4; ++j)
            add_edge(i, j, g);
    return g;
}

// Complete graph K5
Graph build_complete_graph_k5() {
    Graph g(5);
    for (int i = 0; i < 5; ++i)
        for (int j = i + 1; j < 5; ++j)
            add_edge(i, j, g);
    return g;
}

// Star graph
Graph build_star_graph() {
    Graph g(5);
    for (int i = 1; i < 5; ++i)
        add_edge(0, i, g);
    return g;
}

// ------------------------------------------------------------
// Main Test Runner
// ------------------------------------------------------------
int main() {
    int passed = 0;
    int total = 0;

    try {
        // Test cases with expected matching sizes
        std::vector<std::tuple<Graph, std::string, int>> test_cases = {
            {build_empty_graph(), "Empty Graph", 0},
            {build_single_vertex_graph(), "Single Vertex", 0},
            {build_two_vertices_no_edge(), "Two Vertices No Edge", 0},
            {build_two_vertices_with_edge(), "Two Vertices With Edge", 1},
            {build_path_graph(), "Path Graph P6", 3},
            {build_cycle_graph(), "Cycle Graph C6", 3},
            {build_complete_graph_k4(), "Complete Graph K4", 2},
            {build_complete_graph_k5(), "Complete Graph K5", 2},
            {build_star_graph(), "Star Graph", 1}
        };

        for (auto& [graph, name, expected] : test_cases) {
            total++;
            if (test_matching(graph, name, expected)) {
                passed++;
            }
        }

        std::cout << "========================================\n";
        std::cout << "Test Summary: " << passed << "/" << total << " tests passed\n";

        if (passed == total) {
            std::cout << "All tests PASSED!\n";
            return 0;
        } else {
            std::cout << "Some tests FAILED!\n";
            return 1;
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
}