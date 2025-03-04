#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

vector<vector<int> > import_graph(string file_name) {
    ifstream fin(file_name);
    if (!fin.is_open()) {
        cerr << "Unable to open file." << endl;
        exit(1);
    }
    int num_vertices, num_edges;
    fin >> num_vertices >> num_edges;
    vector<vector<int> > adjmatrix(num_vertices,vector<int>(num_vertices,0));
    int vertex_1,vertex_2;
    for (int i = 0;i<num_edges;i++) {
        fin >> vertex_1 >> vertex_2;
        adjmatrix[vertex_1][vertex_2] = 1;
        adjmatrix[vertex_2][vertex_1] = 1;
    }
    return adjmatrix;
}

int main(int argc, char* argv[]) {
    vector<vector<int> > adjmatrix = import_graph(argv[1]);
    for (int i = 0;i<(int)adjmatrix.size();i++) {
        for (int j = 0;j < 0; j++) {
            cout << adjmatrix[i][j] << " ";
        }
        cout << '\n';
    }
    cout << '\n';
    return 0;
}
