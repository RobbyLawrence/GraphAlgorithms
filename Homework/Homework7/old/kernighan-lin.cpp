#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

class Graph { // I wanted to take a class-based approach this week
private:
    int num_vertices;
    int num_edges;
    vector<vector<int> > adjlist;

public:
    Graph(int n, int m) : num_vertices(n), num_edges(m) {
        adjlist.resize(n);
    }

    void add_edge(int u, int v) {
        adjlist[u].push_back(v);
        adjlist[v].push_back(u);
    }

    int get_num_vertices() const {
        return num_vertices;
    }

    const vector<int>& get_neighbors(int vertex) const {
        return adjlist[vertex];
    }

    bool is_edge(int u, int v) const { // useful later on
        const auto& neighbors = adjlist[u];
        return find(neighbors.begin(), neighbors.end(), v) != neighbors.end();
    }
};

class KernighanLin {
private:
    const Graph& graph;
    vector<bool> partition;  // true for A, false for B
    int cut_size; // number of edges crossing

    int d_calc(int vertex) { // calculates d value for a single vertex
        int internal = 0; // edges to vertices inside partition
        int external = 0; // vice versa
        bool is_in_a = partition[vertex];

        for (int neighbor : graph.get_neighbors(vertex)) {
            if (partition[neighbor] == is_in_a) {
                internal++;
            } else {
                external++;
            }
        }

        return external - internal;
    }

    int calc_gain(int a, int b) {
        if (partition[a] == partition[b]) { // vertices shouldn't be in the same partition
            return 0;
        }
        int gain = d_calc(a) + d_calc(b);
        if (graph.is_edge(a, b)) {
            gain -= 2; // lose two from edge
        }
        return gain;
    }

    int calc_cut() { // calculate initial cut
        int count = 0;
        for (int i = 0; i < graph.get_num_vertices(); i++) {
            for (int neighbor : graph.get_neighbors(i)) {
                if (partition[i] != partition[neighbor]) {
                    count++;
                }
            }
        }
        return count / 2; // each edge should get counted twice
    }

public:
    int total_iterations;
    int total_passes;
    KernighanLin(const Graph& g) : graph(g) {
        int n = graph.get_num_vertices();
        partition.resize(n); // for balancing partitions
        for (int i = 0; i < n; i++) {
            partition[i] = (i < n / 2); // first half in A, second half in B
        }
        cut_size = calc_cut();
    }

    // Run the Kernighan-Lin algorithm
    void run_algorithm() {
        int num_vertices = graph.get_num_vertices();

        bool improved = true;
        while (improved) { // also could've done a do while loop
            // but who actually uses do while loops
            improved = false;
            total_iterations++;
            vector<bool> available(num_vertices, true); // tracks which vertices are still able to be swapped
            vector<int> gain_values;
            vector<int> a_vertices;
            vector<int> b_vertices;
            // i'm making num_vertices / 10 passes since that worked well during testing
            for (int pass = 0; pass < num_vertices / 10; pass++) {
                total_passes++;
                int best_a = -1, best_b = -1;
                int best_gain = numeric_limits<int>::min();
                for (int a = 0; a < num_vertices; a++) {
                    if (!available[a] || !partition[a]) {
                        continue;
                    }
                    for (int b = 0; b < num_vertices; b++) {
                        if (!available[b] || partition[b]) {
                            continue;
                        }
                        int gain = calc_gain(a, b);
                        if (gain > best_gain) {
                            best_gain = gain;
                            best_a = a;
                            best_b = b;
                        }
                    }
                }
                gain_values.push_back(best_gain);
                a_vertices.push_back(best_a);
                b_vertices.push_back(best_b);

                available[best_a] = available[best_b] = false; // mark as used
                partition[best_a] = false;
                partition[best_b] = true;
            }

            for (int i = 0; i < num_vertices / 10; i++) { // undo the temp swaps
                partition[a_vertices[i]] = true;
                partition[b_vertices[i]] = false;
            }
            int max_k = -1;
            int max_gain = 0;
            int current_gain = 0;

            for (int k = 0; k < num_vertices / 10; k++) {
                current_gain += gain_values[k];
                if (current_gain > max_gain) {
                    max_gain = current_gain;
                    max_k = k;
                }
            }

            if (max_gain > 0) { // make the swap if the gain was good
                improved = true;
                for (int k = 0; k <= max_k; k++) {
                    partition[a_vertices[k]] = false;
                    partition[b_vertices[k]] = true;
                }
                cut_size -= max_gain;
            }
        }
    }
    // final partitions
    pair<vector<int>, vector<int> > get_partitions() const {
        vector<int> a, b;
        for (int i = 0; i < graph.get_num_vertices(); i++) {
            if (partition[i]) {
                a.push_back(i);
            } else {
                b.push_back(i);
            }
        }
        return make_pair(a, b);
    }

    int get_cut_size() const {
        return cut_size;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "usage: ./kernighan-lin <file_name>" << endl;
        return 1;
    }

    ifstream fin(argv[1]);
    if (!fin.is_open()) {
        cerr << "Unable to open file." << endl;
        return 1;
    }

    int num_vertices, num_edges;
    fin >> num_vertices >> num_edges;

    Graph graph(num_vertices, num_edges);

    for (int i = 0; i < num_edges; i++) {
        int u, v;
        fin >> u >> v;
        graph.add_edge(u, v);
    }

    KernighanLin kl(graph);
    kl.run_algorithm();

    auto partitions = kl.get_partitions();
    int cut_size = kl.get_cut_size();

    cout << "Cut size: " << cut_size << endl;
    cout << "Iterations: " << kl.total_iterations << endl;
    cout << "Total passes: " << kl.total_passes << endl;
    cout << "Partition A: ";
    for (int vertex : partitions.first) {
        cout << vertex << " ";
    }
    cout << endl;

    cout << "Partition B: ";
    for (int vertex : partitions.second) {
        cout << vertex << " ";
    }
    cout << endl;

    return 0;
}
