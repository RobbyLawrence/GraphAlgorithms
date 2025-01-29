#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

int main(int argc, char* argv[]) {
  // error check
  if (argc != 2) {
    cerr << "usage: ./dimacs file" << endl;
    return 1;
  }
  // open file
  ifstream fin(argv[1]);
  if (!fin.is_open()) {
    cerr << "Unable to open file" << endl;
    return 1;
  }
  int num_vertices,num_edges = 0;
  fin >> num_vertices >> num_edges;
  // create adjacency matrix
  vector<vector<int> > adjmatrix;
  adjmatrix.resize(num_vertices,vector<int>(num_vertices,0));
  int vertex1, vertex2;
  while (fin >> vertex1 >> vertex2) {
    adjmatrix[vertex1][vertex2] = 1;
    adjmatrix[vertex2][vertex1] = 1;
  }
  // output
  for (int i = 0;i<num_vertices;i++) {
    for (int j = 0;j<num_vertices;j++) {
      cout << adjmatrix[i][j] << " ";
    }
    cout << '\n';
  }
  return 0;
}
