#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

class Graph { // I wanted to use a class-based approach this week
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

    bool is_edge(int u, int v) const { // this will be useful later
        const vector<int>& neighbors = adjlist[u];
        return find(neighbors.begin(), neighbors.end(), v) != neighbors.end();
    }
};

class KernighanLin {
private:
    const Graph& graph;
    vector<bool> partition; // true for A, false for B
    int cut_size;
    int total_iterations;
    int total_passes;
    int calculate_d_value(int vertex) { // calculates d value for single vertex
        int internal = 0; // num of edges to vertices in same partition
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

    int calc_gain(int a, int b) { // simulates the gain if we swapped a and b
        if (partition[a] == partition[b]) { // vertices can't be in same partition
            return 0;
        }
        int gain = calculate_d_value(a) + calculate_d_value(b);
        if (graph.is_edge(a, b)) { // lose two from edge contribution
            gain -= 2;
        }
        return gain;
    }
    int calc_cut() { // find initial cut size
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
    KernighanLin(const Graph& g) : graph(g), total_iterations(0), total_passes(0) {
        int num_vertices = graph.get_num_vertices();

        partition.resize(num_vertices); // need balanced partition
        for (int i = 0; i < num_vertices; i++) {
            partition[i] = (i < num_vertices / 2);  // place first half in A, second half in B
        }
        cut_size = calc_cut();
    }

    void run_algorithm() {
        int num_vertices = graph.get_num_vertices();
        bool improved = true;

        while (improved) { // run the algorithm until we can't find a better solution
            improved = false;
            total_iterations++;
            vector<bool> available(num_vertices, true); // record of available vertices
            vector<int> gain_values;
            vector<int> a_vertices;
            vector<int> b_vertices;
            // chose 10 passes since it ran quickly on graphs ~100 vertices and returned accurate results
            for (int pass = 0; pass < 10; pass++) {
                total_passes++;
                int best_a = -1, best_b = -1;
                int best_gain = numeric_limits<int>::min();
                for (int a = 0; a < num_vertices; a++) { // find best swap
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
                // mark vertices as used
                available[best_a] = available[best_b] = false;
                partition[best_a] = false;
                partition[best_b] = true;
            }

            for (int i = 0; i < 10; i++) { // undo the temp swaps
                partition[a_vertices[i]] = true;
                partition[b_vertices[i]] = false;
            }
            int max_k = -1; // find max
            int max_gain = 0;
            int current_gain = 0;
            for (int k = 0; k < 10; k++) {
                current_gain += gain_values[k];
                if (current_gain > max_gain) {
                    max_gain = current_gain;
                    max_k = k;
                }
            }
            if (max_gain > 0) { // perform swaps if the gain is positive
                improved = true;
                for (int k = 0; k <= max_k; k++) {
                    partition[a_vertices[k]] = false;
                    partition[b_vertices[k]] = true;
                }
                cut_size -= max_gain;
            }
        }
    }

    pair<vector<int>, vector<int> > get_partitions() const { // grab the final partitions
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

    pair<int, int> get_iteration_stats() const { // just use this to get the private members of Kernighan-Lin
        return make_pair(total_iterations, total_passes);
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

    for (int i = 0; i < num_edges; i++) { // import graph
        int u, v;
        fin >> u >> v;
        graph.add_edge(u, v);
    }
    KernighanLin kl(graph);
    kl.run_algorithm();
    pair<vector<int>,vector<int> > partitions = kl.get_partitions();
    int cut_size = kl.get_cut_size();
    pair<int,int> iteration_stats = kl.get_iteration_stats();
    cout << "Cut size: " << cut_size << endl;
    cout << "Iterations: " << iteration_stats.first << endl;
    cout << "Total passes: " << iteration_stats.second << endl;

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
