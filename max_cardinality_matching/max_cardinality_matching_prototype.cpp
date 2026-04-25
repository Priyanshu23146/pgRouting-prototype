// Compile:
// g++ -std=c++17 -Wall -Wextra max_cardinality_matching_prototype.cpp -o max_cardinality_matching_prototype -lboost_graph
//
// Standalone prototype for pgr_maxCardinalityMatching using Boost Graph Library
// Demonstrates matching, validation, and interpretation

#include <iostream>
#include <vector>
#include <iomanip>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using Graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS>;

using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

// ------------------------------------------------------------
// Utility and graph builder functions organized in a namespace
namespace matching_utils {

// Print all edges of the graph
void print_graph(const Graph& g) {
    std::cout << "=== Graph Edges ===\n";
    for (auto edge : boost::make_iterator_range(edges(g))) {
        std::cout << source(edge, g) << " - " << target(edge, g) << "\n";
    }
    std::cout << "\n";
}

// Print the matching result
void print_matching(const Graph& g, const std::vector<Vertex>& mate) {
    std::cout << "Matching pairs:\n";
    for (size_t i = 0; i < mate.size(); ++i) {
        if (mate[i] != boost::graph_traits<Graph>::null_vertex() && i < mate[i]) {
            std::cout << i << " <--> " << mate[i] << "\n";
        }
    }
    std::cout << "\n";
}

// Print statistics about the matching
void analyze_matching(const Graph& g, const std::vector<Vertex>& mate) {
    int V = boost::num_vertices(g);
    int matching_size = boost::matching_size(g, &mate[0]);
    std::cout << "Matching size = " << matching_size << "\n";
    std::cout << "Unmatched vertices = " << (V - 2 * matching_size) << "\n";
    std::cout << "Check: matching_size <= V/2 = "
              << matching_size << " <= " << V / 2 << "\n\n";
}

// Path graph builder
Graph build_path_graph() {
    Graph g(6);
    add_edge(0,1,g);
    add_edge(1,2,g);
    add_edge(2,3,g);
    add_edge(3,4,g);
    add_edge(4,5,g);
    return g;
}

// Cycle graph builder
Graph build_cycle_graph() {
    Graph g(6);
    for (int i = 0; i < 6; ++i)
        add_edge(i, (i + 1) % 6, g);
    return g;
}

// Complete graph K4 builder
Graph build_complete_graph() {
    Graph g(4);
    for (int i = 0; i < 4; ++i)
        for (int j = i + 1; j < 4; ++j)
            add_edge(i, j, g);
    return g;
}

} // namespace matching_utils

// ------------------------------------------------------------
// Core Function: Run Matching Algorithm Demo
void run_matching_demo(Graph& g, const std::string& name) {
    using namespace matching_utils;
    int V = boost::num_vertices(g);
    int E = boost::num_edges(g);

    std::cout << "========================================\n";
    std::cout << "Graph: " << name << "\n";
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
}

// ------------------------------------------------------------
// ...graph builders moved to matching_utils namespace above...

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main() {
    using namespace matching_utils;
    try {
        Graph path = build_path_graph();
        run_matching_demo(path, "Path Graph");

        Graph cycle = build_cycle_graph();
        run_matching_demo(cycle, "Cycle Graph");

        Graph complete = build_complete_graph();
        run_matching_demo(complete, "Complete Graph K4");

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}