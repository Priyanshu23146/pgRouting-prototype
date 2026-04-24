// graph_utils.hpp
// Common utilities for graph algorithms in pgRouting prototypes

#ifndef GRAPH_UTILS_HPP
#define GRAPH_UTILS_HPP

#include <iostream>
#include <vector>
#include <set>
#include <boost/graph/adjacency_list.hpp>

using Graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS>;

using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

// Print graph edges
void print_graph(const Graph& g) {
    std::cout << "=== Graph Edges ===\n";
    for (auto e : boost::make_iterator_range(edges(g))) {
        std::cout << source(e, g) << " - " << target(e, g) << "\n";
    }
    std::cout << "\n";
}

// Get basic graph statistics
struct GraphStats {
    int vertices;
    int edges;
    double density;
    bool is_connected;
};

GraphStats get_graph_stats(const Graph& g) {
    int V = num_vertices(g);
    int E = num_edges(g);
    double density = V > 1 ? 2.0 * E / (V * (V - 1)) : 0.0;

    // Simple connectivity check (DFS from vertex 0)
    bool connected = true;
    if (V > 1) {
        std::vector<bool> visited(V, false);
        std::function<void(Vertex)> dfs = [&](Vertex u) {
            visited[u] = true;
            for (auto e : boost::make_iterator_range(out_edges(u, g))) {
                Vertex v = target(e, g);
                if (!visited[v]) dfs(v);
            }
        };
        dfs(0);
        for (bool v : visited) {
            if (!v) {
                connected = false;
                break;
            }
        }
    }

    return {V, E, density, connected};
}

// Print graph statistics
void print_graph_stats(const Graph& g) {
    auto stats = get_graph_stats(g);
    std::cout << "Graph Statistics:\n";
    std::cout << "  Vertices: " << stats.vertices << "\n";
    std::cout << "  Edges: " << stats.edges << "\n";
    std::cout << "  Density: " << std::fixed << std::setprecision(3) << stats.density << "\n";
    std::cout << "  Connected: " << (stats.is_connected ? "Yes" : "No") << "\n\n";
}

// Validate matching result
bool validate_matching(const Graph& g, const std::vector<Vertex>& mate) {
    std::set<Vertex> matched;

    for (size_t i = 0; i < mate.size(); ++i) {
        Vertex partner = mate[i];
        if (partner != boost::graph_traits<Graph>::null_vertex()) {
            // Check for conflicts
            if (matched.count(i) || matched.count(partner)) {
                return false;
            }
            matched.insert(i);
            matched.insert(partner);

            // Check if edge exists
            if (!boost::edge(i, partner, g).second) {
                return false;
            }
        }
    }
    return true;
}

#endif // GRAPH_UTILS_HPP