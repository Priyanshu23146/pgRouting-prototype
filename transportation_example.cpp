// transportation_example.cpp
// Real-world example: Transportation network analysis

#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include "graph_utils.hpp"

using Graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS>;

using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

// Example: Ride-sharing matching in a city
void ride_sharing_example() {
    std::cout << "=== Ride-Sharing Matching Example ===\n";
    std::cout << "Scenario: 8 riders and 8 drivers in a city\n\n";

    // Create a bipartite graph: riders (0-3) and drivers (4-7)
    Graph g(8);

    // Add possible matches (edges represent compatibility/distance constraints)
    add_edge(0, 4, g); // Rider 0 can be matched with Driver 0
    add_edge(0, 5, g); // Rider 0 can be matched with Driver 1
    add_edge(1, 4, g); // Rider 1 can be matched with Driver 0
    add_edge(1, 6, g); // Rider 1 can be matched with Driver 2
    add_edge(2, 5, g); // Rider 2 can be matched with Driver 1
    add_edge(2, 7, g); // Rider 2 can be matched with Driver 3
    add_edge(3, 6, g); // Rider 3 can be matched with Driver 2
    add_edge(3, 7, g); // Rider 3 can be matched with Driver 3

    print_graph_stats(g);

    // Find maximum matching
    std::vector<Vertex> mate(num_vertices(g));
    boost::edmonds_maximum_cardinality_matching(g, &mate[0]);

    int matching_size = boost::matching_size(g, &mate[0]);

    std::cout << "Maximum Matching Results:\n";
    std::cout << "Matching size: " << matching_size << " pairs\n\n";

    std::cout << "Rider-Driver assignments:\n";
    for (size_t i = 0; i < 4; ++i) { // Riders 0-3
        Vertex driver = mate[i];
        if (driver != boost::graph_traits<Graph>::null_vertex()) {
            std::cout << "Rider " << i << " -> Driver " << (driver - 4) << "\n";
        } else {
            std::cout << "Rider " << i << " -> No driver assigned\n";
        }
    }

    std::cout << "\nUnmatched riders: " << (4 - matching_size) << "\n";
    std::cout << "Unmatched drivers: " << (4 - matching_size) << "\n\n";
}

// Example: Network vulnerability analysis
void network_vulnerability_example() {
    std::cout << "=== Network Vulnerability Analysis ===\n";
    std::cout << "Scenario: Transportation network with potential articulation points\n\n";

    // Create a road network graph
    Graph g(8);

    // Main roads forming a connected network
    add_edge(0, 1, g);
    add_edge(1, 2, g);
    add_edge(2, 3, g);
    add_edge(3, 4, g);
    add_edge(4, 5, g);
    add_edge(5, 6, g);
    add_edge(6, 7, g);

    // Additional connections creating potential vulnerabilities
    add_edge(1, 3, g); // Bridge connection
    add_edge(2, 4, g); // Another bridge

    print_graph_stats(g);

    // Analyze biconnected components
    std::vector<int> component(num_edges(g));
    auto e_index = get(boost::edge_index, g);
    int idx = 0;
    for (auto e : boost::make_iterator_range(edges(g))) {
        put(e_index, e, idx++);
    }

    boost::biconnected_components(g, make_iterator_property_map(component.begin(), e_index));

    std::set<int> unique_comps(component.begin(), component.end());
    std::cout << "Biconnected components: " << unique_comps.size() << "\n";

    // Find articulation points (critical junctions)
    std::vector<Vertex> articulations;
    for (Vertex v = 0; v < num_vertices(g); ++v) {
        std::vector<bool> visited(num_vertices(g), false);
        std::function<void(Vertex)> dfs = [&](Vertex u) {
            visited[u] = true;
            for (auto e : boost::make_iterator_range(out_edges(u, g))) {
                Vertex t = target(e, g);
                if (t == u) t = source(e, g);
                if (t == v) continue;
                if (!visited[t]) dfs(t);
            }
        };

        Vertex start = (v == 0) ? 1 : 0;
        dfs(start);

        int count = 0;
        for (Vertex u = 0; u < num_vertices(g); ++u) {
            if (u != v && !visited[u]) {
                count++;
                break;
            }
        }

        if (count > 0) articulations.push_back(v);
    }

    std::cout << "Critical junctions (articulation points): ";
    if (articulations.empty()) {
        std::cout << "None - Network is robust\n";
    } else {
        for (size_t i = 0; i < articulations.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << articulations[i];
        }
        std::cout << " - These points are critical for network connectivity\n";
    }
    std::cout << "\n";
}

int main() {
    try {
        ride_sharing_example();
        network_vulnerability_example();

        std::cout << "=== Examples completed successfully ===\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}