// Compile:
// g++ -std=c++17 -Wall -Wextra test_biconnected_components.cpp -o test_biconnected_components -lboost_graph
//
// Test file for biconnected components and articulation points prototype
// Tests various graph structures and validates biconnected components and articulation points

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

using Graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS,
    boost::no_property,
    boost::property<boost::edge_index_t, int>>;

using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

void print_graph(const Graph& g) {
    std::cout << "=== Graph Edges ===\n";
    for (auto e : boost::make_iterator_range(edges(g))) {
        std::cout << source(e, g) << " - " << target(e, g) << "\n";
    }
    std::cout << "\n";
}

std::vector<Vertex> find_articulation_points(const Graph& g) {
    std::vector<Vertex> articulations;
    auto n = num_vertices(g);

    for (Vertex v = 0; v < n; ++v) {
        std::vector<bool> visited(n, false);
        std::function<void(Vertex)> dfs = [&](Vertex u) {
            visited[u] = true;
            for (auto e : boost::make_iterator_range(out_edges(u, g))) {
                Vertex t = target(e, g);
                if (t == u) t = source(e, g); // safe for undirected
                if (t == v) continue;
                if (!visited[t]) dfs(t);
            }
        };

        Vertex start = -1;
        for (Vertex u = 0; u < n; ++u) {
            if (u == v) continue;
            start = u;
            break;
        }
        if (start == Vertex(-1)) continue;

        dfs(start);

        int count = 0;
        for (Vertex u = 0; u < n; ++u) {
            if (u == v) continue;
            if (!visited[u]) {
                count++;
                break;
            }
        }

        if (count > 0) articulations.push_back(v);
    }

    return articulations;
}

// ------------------------------------------------------------
// Test Function: Run Biconnected Components Analysis and Validate
// ------------------------------------------------------------
bool test_biconnected_components(Graph& g, const std::string& name,
                                int expected_components, const std::vector<Vertex>& expected_articulations) {
    int V = num_vertices(g);
    int E = num_edges(g);

    std::cout << "========================================\n";
    std::cout << "Testing: " << name << "\n";
    std::cout << "Vertices V = " << V << ", Edges E = " << E << "\n\n";

    print_graph(g);

    // Set up edge indices for biconnected components
    auto e_index = get(boost::edge_index, g);
    int idx = 0;
    for (auto e : boost::make_iterator_range(edges(g))) {
        put(e_index, e, idx++);
    }

    std::vector<int> component(num_edges(g));
    boost::biconnected_components(g,
        make_iterator_property_map(component.begin(), e_index));

    std::vector<Vertex> art_points = find_articulation_points(g);

    std::cout << "Edge component assignments:\n";
    idx = 0;
    for (auto e : boost::make_iterator_range(edges(g))) {
        std::cout << source(e, g) << " - " << target(e, g)
                  << " (component=" << component[idx] << ")\n";
        ++idx;
    }
    std::cout << "\n";

    std::cout << "Articulation points: ";
    if (art_points.empty()) {
        std::cout << "none";
    } else {
        for (size_t i = 0; i < art_points.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << art_points[i];
        }
    }
    std::cout << "\n";

    std::set<int> unique_comps(component.begin(), component.end());
    int actual_components = unique_comps.size();
    std::cout << "Biconnected components count = " << actual_components << "\n";

    // Sort articulation points for comparison
    std::vector<Vertex> sorted_art_points = art_points;
    std::sort(sorted_art_points.begin(), sorted_art_points.end());
    std::vector<Vertex> sorted_expected = expected_articulations;
    std::sort(sorted_expected.begin(), sorted_expected.end());

    bool components_match = (actual_components == expected_components);
    bool articulations_match = (sorted_art_points == sorted_expected);

    if (components_match && articulations_match) {
        std::cout << "TEST PASSED: Components and articulation points match expected values\n\n";
        return true;
    } else {
        std::cout << "TEST FAILED:\n";
        if (!components_match) {
            std::cout << "  Expected components: " << expected_components
                      << ", got: " << actual_components << "\n";
        }
        if (!articulations_match) {
            std::cout << "  Expected articulations: [";
            for (size_t i = 0; i < sorted_expected.size(); ++i) {
                if (i > 0) std::cout << ", ";
                std::cout << sorted_expected[i];
            }
            std::cout << "], got: [";
            for (size_t i = 0; i < sorted_art_points.size(); ++i) {
                if (i > 0) std::cout << ", ";
                std::cout << sorted_art_points[i];
            }
            std::cout << "]\n";
        }
        std::cout << "\n";
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

// Path graph P4
Graph build_path_graph_p4() {
    Graph g(4);
    add_edge(0, 1, g);
    add_edge(1, 2, g);
    add_edge(2, 3, g);
    return g;
}

// Cycle graph C4
Graph build_cycle_graph_c4() {
    Graph g(4);
    add_edge(0, 1, g);
    add_edge(1, 2, g);
    add_edge(2, 3, g);
    add_edge(3, 0, g);
    return g;
}

// Sample graph from prototype:
// 0--1--2
// | /    |
// 3      4
Graph build_sample_graph() {
    Graph g(5);
    add_edge(0, 1, g);
    add_edge(1, 2, g);
    add_edge(0, 3, g);
    add_edge(1, 3, g);
    add_edge(2, 4, g);
    return g;
}

// Complete graph K4
Graph build_complete_graph_k4() {
    Graph g(4);
    add_edge(0, 1, g);
    add_edge(0, 2, g);
    add_edge(0, 3, g);
    add_edge(1, 2, g);
    add_edge(1, 3, g);
    add_edge(2, 3, g);
    return g;
}

// Star graph
Graph build_star_graph() {
    Graph g(5);
    add_edge(0, 1, g);
    add_edge(0, 2, g);
    add_edge(0, 3, g);
    add_edge(0, 4, g);
    return g;
}

// ------------------------------------------------------------
// Main Test Runner
// ------------------------------------------------------------
int main() {
    int passed = 0;
    int total = 0;

    try {
        // Test cases with expected component counts and articulation points
        std::vector<std::tuple<Graph, std::string, int, std::vector<Vertex>>> test_cases = {
            {build_empty_graph(), "Empty Graph", 0, {}},
            {build_single_vertex_graph(), "Single Vertex", 0, {}},
            {build_two_vertices_no_edge(), "Two Vertices No Edge", 0, {}},
            {build_two_vertices_with_edge(), "Two Vertices With Edge", 1, {}},
            {build_path_graph_p4(), "Path Graph P4", 3, {1, 2}},  // vertices 1 and 2 are articulations
            {build_cycle_graph_c4(), "Cycle Graph C4", 1, {}},    // no articulations in a cycle
            {build_sample_graph(), "Sample Graph", 3, {1, 2}},     // vertices 1 and 2 are articulation points
            {build_complete_graph_k4(), "Complete Graph K4", 1, {}}, // no articulations in complete graph
            {build_star_graph(), "Star Graph", 4, {0}}            // center vertex 0 is articulation
        };

        for (auto& test_case : test_cases) {
            Graph graph = std::get<0>(test_case);
            std::string name = std::get<1>(test_case);
            int expected_components = std::get<2>(test_case);
            std::vector<Vertex> expected_articulations = std::get<3>(test_case);
            total++;
            if (test_biconnected_components(graph, name, expected_components, expected_articulations)) {
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