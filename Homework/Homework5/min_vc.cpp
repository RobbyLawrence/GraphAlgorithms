#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

// Function to check if a given set of vertices is a vertex cover
bool vc_check(vector<vector<int> > &graph, set<int> &cover) {
    for (size_t u = 0; u < graph.size(); ++u) {
        for (int v : graph[u]) {
            if (cover.find(u) == cover.end() && cover.find(v) == cover.end()) {
                return false;
            }
        }
    }
    return true;
}

bool find_vertex_cover(vector<vector<int> > &graph, set<int> &cover, int k, int start) {
    if (cover.size() == k) {
        return vc_check(graph, cover);
    }

    for (int i = start; i < graph.size(); ++i) {
        cover.insert(i);
        if (find_vertex_cover(graph, cover, k, i + 1)) {
            return true;
        }
        cover.erase(i);
    }

    return false;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <graph_file.dim>" << endl;
        return 1;
    }

    ifstream infile(argv[1]);
    if (!infile) {
        cerr << "Error: Unable to open file " << argv[1] << endl;
        return 1;
    }

    int num_vertices, num_edges;
    infile >> num_vertices >> num_edges;

    vector<vector<int>> graph(num_vertices);
    int u, v;
    while (infile >> u >> v) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    set<int> cover;
    bool found = false;

    // Try to find a vertex cover of size 10 or less
    for (int k = 1; k <= 10; ++k) {
        if (find_vertex_cover(graph, cover, k, 0)) {
            found = true;
            break;
        }
    }

    if (found) {
        cout << "Size of vertex cover: " << cover.size() << endl;
        cout << "{";
        for (auto it = cover.begin(); it != cover.end(); ++it) {
            if (it != cover.begin()) {
                cout << ", ";
            }
            cout << *it;
        }
        cout << "}" << endl;
    } else {
        cout << "0" << endl;
    }

    return 0;
}
