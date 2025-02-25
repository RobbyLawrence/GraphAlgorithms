#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

bool vc_check(vector<vector<int> > &adjlist, set<int> &cover) {
    for (size_t u = 0; u < adjlist.size(); ++u) {
        for (int v : adjlist[u]) {
            if (cover.find(u) == cover.end() && cover.find(v) == cover.end()) {
                return false;
            }
        }
    }
    return true;
}

bool find_vertex_cover(vector<vector<int> > &adjlist, set<int> &cover, int k, int start) {
    if (cover.size() == k) {
        return vc_check(adjlist, cover);
    }
    for (int i = start; i < adjlist.size(); ++i) {
        cover.insert(i);
        if (find_vertex_cover(adjlist, cover, k, i + 1)) {
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
    ifstream fin(argv[1]);
    if (!fin) {
        cerr << "Unable to open file" << endl;
        return 1;
    }
    int num_vertices, num_edges;
    fin >> num_vertices >> num_edges;
    vector<vector<int> > adjlist(num_vertices);
    int u, v;
    while (fin >> u >> v) {
        adjlist[u].push_back(v);
        adjlist[v].push_back(u);
    }
    set<int> cover;
    bool found = false;
    for (int k = 1; k < 11; k++) {
        if (find_vertex_cover(adjlist, cover, k, 0)) {
            found = true;
            break;
        }
    }
    if (found) {
        cout << "Size of vertex cover: " << cover.size() << endl;
        cout << "{";
        for (set<int>::iterator it = cover.begin(); it != cover.end(); ++it) {
            if (it != cover.begin()) {
                cout << ", ";
            }
            cout << *it;
        }
        cout << "}" << endl;
    } else { // if we can't find a minimum vertex cover with size < 10
        cout << "0" << endl;
    }
    return 0;
}
