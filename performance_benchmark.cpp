// performance_benchmark.cpp
// Performance benchmarking utility for pgRouting prototypes

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using Graph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS>;

using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

// Generate random graph with specified density
Graph generate_random_graph(int vertices, double density) {
    Graph g(vertices);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < vertices; ++i) {
        for (int j = i + 1; j < vertices; ++j) {
            if (dis(gen) < density) {
                add_edge(i, j, g);
            }
        }
    }
    return g;
}

// Benchmark max cardinality matching
double benchmark_matching(const Graph& g, int iterations = 10) {
    std::vector<double> times;

    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<Vertex> mate(num_vertices(g));
        boost::edmonds_maximum_cardinality_matching(g, &mate[0]);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        times.push_back(duration.count());
    }

    // Return average time
    double sum = 0;
    for (double t : times) sum += t;
    return sum / times.size();
}

int main() {
    std::cout << "=== pgRouting Prototype Performance Benchmark ===\n\n";

    std::vector<std::pair<int, double>> test_cases = {
        {10, 0.3},   // Small sparse graph
        {10, 0.8},   // Small dense graph
        {50, 0.1},   // Medium sparse graph
        {50, 0.3},   // Medium graph
        {100, 0.05}, // Large sparse graph
        {100, 0.1}   // Large graph
    };

    std::cout << "Graph Size | Density | Avg Time (ms) | Vertices | Edges\n";
    std::cout << "-----------|---------|---------------|----------|-------\n";

    for (auto& [vertices, density] : test_cases) {
        Graph g = generate_random_graph(vertices, density);
        double avg_time = benchmark_matching(g);

        std::cout << std::setw(10) << vertices << " | "
                  << std::setw(7) << density << " | "
                  << std::setw(13) << std::fixed << std::setprecision(2) << avg_time << " | "
                  << std::setw(8) << vertices << " | "
                  << std::setw(6) << num_edges(g) << "\n";
    }

    std::cout << "\nBenchmark completed.\n";
    return 0;
}