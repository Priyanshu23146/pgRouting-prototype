#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
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

void run_biconnected_demo() {
    // Sample graph:
    // 0--1--2
    // | /    |
    // 3      4

    Graph g(5);
    add_edge(0, 1, g);
    add_edge(1, 2, g);
    add_edge(0, 3, g);
    add_edge(1, 3, g);
    add_edge(2, 4, g);

    auto e_index = get(boost::edge_index, g);
    int idx = 0;
    for (auto e : boost::make_iterator_range(edges(g))) {
        put(e_index, e, idx++);
    }

    std::vector<int> component(num_edges(g));
    boost::biconnected_components(g,
        make_iterator_property_map(component.begin(), e_index));

    std::vector<Vertex> art_points = find_articulation_points(g);

    std::cout << "=== Biconnected Components Demo ===\n";
    std::cout << "Vertices V = " << num_vertices(g)
              << ", Edges E = " << num_edges(g) << "\n\n";

    print_graph(g);

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
    std::cout << "Biconnected components count = " << unique_comps.size() << "\n";
    std::cout << "\n";
}

int main() {
    try {
        run_biconnected_demo();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}