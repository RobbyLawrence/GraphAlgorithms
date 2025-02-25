#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

vector<vector<int> > import_graph(string file) { // import the graph as a 2D vector of ints
    ifstream fin(file);
    vector<vector<int> > adjmatrix;
    if (!fin.is_open()) {
        cerr << "Unable to open file" << endl;
        return adjmatrix;
    }
    int num_vertices, num_edges = 0;
    fin >> num_vertices >> num_edges;
    adjmatrix.resize(num_vertices, vector<int>(num_vertices, 0));
    int vertex1, vertex2;
    while (fin >> vertex1 >> vertex2) {
        adjmatrix[vertex1][vertex2] = 1;
        adjmatrix[vertex2][vertex1] = 1;
    }
    fin.close();
    return adjmatrix;
}

void find_maximal_cliques(const vector<vector<int> > &adjmatrix, vector<int> &current_clique, vector<int> &candidates, vector<vector<int> > &all_cliques) {
    if (candidates.empty()) {
        if (current_clique.size() > 1) {
            all_cliques.push_back(current_clique);
        }
        return;
    }

    while (!candidates.empty()) {
        // this is similar behavior to a queue, but I didn't want to use a queue because it doesn't support indexing
        int v = candidates.back();
        candidates.pop_back();

        vector<int> new_candidates;
        for (int u : candidates) {
            if (adjmatrix[v][u]) { // test if a vertex i
                new_candidates.push_back(u);
            }
        }
        current_clique.push_back(v);
        find_maximal_cliques(adjmatrix, current_clique, new_candidates, all_cliques); // recursive call
        current_clique.pop_back();
    }
}

int main(int argc, char* argv[]) {
    // error checking
    if (argc != 2) {
        cerr << "usage: ./greedy_clique <file_name>" << endl;
        return 1;
    }

    ifstream fin(argv[1]);
    if (!fin.is_open()) {
        cerr << "Unable to open file." << endl;
        return 1;
    }
    // import graph
    stringstream ss;
    string file_name;
    ss << argv[1];
    ss >> file_name;
    vector<vector<int> > adjmatrix = import_graph(file_name);
    size_t num_vertices = adjmatrix.size();
    vector<vector<int> > all_cliques;

    for (size_t v = 0; v < num_vertices; ++v) {
        vector<int> current_clique;
        current_clique.push_back(v);

        vector<int> candidates;
        for (int u = v + 1; u < num_vertices; ++u) {
            if (adjmatrix[v][u]) {
                candidates.push_back(u);
            }
        }

        find_maximal_cliques(adjmatrix, current_clique, candidates, all_cliques);
    }
    // find the maximum cliques
    vector<vector<int> > max_cliques;
    size_t max_clique_size = 0;
    for (auto clique : all_cliques) {
        if (clique.size() > max_clique_size) {
            max_cliques.clear();
            max_cliques.push_back(clique);
            max_clique_size = clique.size();
        }
        else if (clique.size() == max_clique_size) {
            max_cliques.push_back(clique);
        }
    }
    if (max_cliques.size() == 1) { // there's only one maximum clique
        sort(max_cliques.begin(), max_cliques.end());
        cout << "Maximum clique size: " << max_clique_size << endl;
        cout << "Vertices in the maximum clique: ";
        for (int vertex : max_cliques[0]) {
            cout << vertex << " ";
        }
        cout << endl;
    }
    else {
        cout << "Maximum clique size: " << max_clique_size << endl;
        cout << "There are " << max_cliques.size() << " maximum cliques." << endl;
        for (size_t i = 0; i < max_cliques.size();i++) {
            cout << "Maximum clique #" << i << " vertices:\n";
            sort(max_cliques[i].begin(),max_cliques[i].end());
            for (int vertex : max_cliques[i]) {
                cout << vertex << " ";
            }
            cout << endl;
        }
    }

    return 0;
}
