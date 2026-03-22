// Compile:
// g++ -std=c++17 -Wall -Wextra edge_connectivity_prototype.cpp -o edge_connectivity_prototype -lboost_graph -I"C:/Users/Anshika/Downloads/boost_1_83_0/boost_1_83_0"
//
// Standalone prototype for pgr_edgeConnectivity using Boost Graph Library
// Demonstrates network robustness analysis

#include <iostream>
#include <vector>
#include <iomanip>
#include <iterator>  // Added for back_inserter
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/edge_connectivity.hpp>

using namespace std;
using namespace boost;



// ------------------------------------------------------------
// Graph Type
// ------------------------------------------------------------
typedef adjacency_list<
    vecS,
    vecS,
    undirectedS> Graph;

typedef graph_traits<Graph>::edge_descriptor Edge;

// ------------------------------------------------------------
// Utility: Print Graph
// ------------------------------------------------------------
void print_graph(const Graph& g) {
    cout << "=== Graph Edges ===\n";

    for (auto e : make_iterator_range(edges(g))) {
        cout << source(e, g) << " - " << target(e, g) << "\n";
    }

    cout << "\n";
}

// ------------------------------------------------------------
// Analysis: Edge Connectivity
// ------------------------------------------------------------
void analyze_connectivity(Graph& g) {
    int V = num_vertices(g);
    int E = num_edges(g);

    cout << "Vertices V = " << V << ", Edges E = " << E << "\n\n";

    print_graph(g);

    // Container for the disconnecting set (edges that disconnect the graph)
    vector<Edge> disconnecting_set;

    // ✅ Correct usage: use back_inserter for output iterator
    int connectivity = edge_connectivity(
        g,
        back_inserter(disconnecting_set)
    );

    cout << "Edge Connectivity: " << connectivity << "\n";

    cout << "Interpretation:\n";
    cout << "Minimum number of edges required to disconnect the graph = "
         << connectivity << "\n";

    if (connectivity == 0) {
        cout << "Graph is already disconnected.\n";
    } else if (connectivity == 1) {
        cout << "Graph is weakly connected (single failure can disconnect it).\n";
    } else {
        cout << "Graph is robust (multiple edge removals required).\n";
    }

    // Optional: Print the disconnecting set
    if (!disconnecting_set.empty()) {
        cout << "Disconnecting set edges:\n";
        for (const auto& e : disconnecting_set) {
            cout << source(e, g) << " - " << target(e, g) << "\n";
        }
    }

    cout << "\n----------------------------------------\n\n";
}

// ------------------------------------------------------------
// Graph Builders
// ------------------------------------------------------------

// Simple road network
Graph build_road_network() {
    Graph g(6);

    add_edge(0,1,g);
    add_edge(1,2,g);
    add_edge(2,3,g);
    add_edge(3,4,g);
    add_edge(4,5,g);
    add_edge(0,5,g);  // cycle
    add_edge(1,4,g);  // cross connection

    return g;
}

// Weak graph (bridge exists)
Graph build_weak_graph() {
    Graph g(5);

    add_edge(0,1,g);
    add_edge(1,2,g);
    add_edge(2,3,g);
    add_edge(3,4,g);  // chain (weak)

    return g;
}

// Complete graph (strong)
Graph build_complete_graph() {
    Graph g(5);

    for (int i = 0; i < 5; ++i)
        for (int j = i + 1; j < 5; ++j)
            add_edge(i, j, g);

    return g;
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main() {
    try {
        cout << "========================================\n";
        cout << " Boost Graph Prototype: Edge Connectivity\n";
        cout << "========================================\n\n";

        Graph road = build_road_network();
        cout << "=== Road Network Graph ===\n";
        analyze_connectivity(road);

        Graph weak = build_weak_graph();
        cout << "=== Weak Graph (Chain) ===\n";
        analyze_connectivity(weak);

        Graph complete = build_complete_graph();
        cout << "=== Complete Graph K5 ===\n";
        analyze_connectivity(complete);

    } catch (const std ::exception & e) {
        cout << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}