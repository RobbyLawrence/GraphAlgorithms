#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>

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
// typical bfs with a queue representing nodes to be searched and a vector of bools to show which vertices are visited
void bfs_search(int start_node, const vector<vector<int> >& adjmatrix, vector<bool>& visited) {
    queue<int> q;
    visited[start_node] = true;
    q.push(start_node);
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        for (int neighbor = 0; neighbor < (int)adjmatrix.size(); neighbor++) {
            if (adjmatrix[current][neighbor] == 1 && !visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor); // all neighbors need to be bfs searched
            }
        }
    }
}
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " file_name" << endl;
        return 1;
    }
    vector<vector<int> > adjmatrix = import_graph(argv[1]);
    if (adjmatrix.empty()) {
        cerr << "Empty graph or unable to read file." << endl;
        return 1;
    }
    int num_vertices = adjmatrix.size();
    vector<bool> visit_record(num_vertices, false);
    int connected_components_count = 0;
    // we're updating the bool vector until it's all true, each time we reach an unreached starting vertex we increase the count
    for (int i = 0; i < num_vertices; i++) {
        if (!visit_record[i]) {
            bfs_search(i, adjmatrix, visit_record);
            connected_components_count++;
        }
    }
    cout << "Number of connected components: " << connected_components_count << endl;
    return 0;
}
