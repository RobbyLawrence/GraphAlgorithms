#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <queue>
#include <algorithm>
#include <map>
using namespace std;

vector<vector<int> > import_graph(string file_name) {
  vector<vector<int> > adjmatrix;
  ifstream fin(file_name);
  if (!fin.is_open()) {
    cerr << "Unable to open file" << endl;
    return adjmatrix; // returns empty adjmatrix
  }
  int num_vertices,num_edges,vertex_1,vertex_2;
  fin >> num_vertices >> num_edges;
  adjmatrix.resize(num_vertices,vector<int>(num_vertices,0));
  // build adjmatrix
  for (int i = 0;i < num_edges;i++) {
    fin >> vertex_1 >> vertex_2;
    adjmatrix[vertex_1][vertex_2] = 1;
    adjmatrix[vertex_2][vertex_1] = 1;
  }
  return adjmatrix;
}

vector<int> find_shortest_path(int start, int end, const vector<vector<int> >& adjmatrix) {
  int n = adjmatrix.size();
  vector<bool> visited(n, false);
  // create vector to keep track of which vertices are each others' parent vertices
  vector<int> parent(n, -1);
  queue<int> q;
  q.push(start);
  visited[start] = true;
  while (!q.empty()) {
    int current = q.front();
    q.pop();
    if (current == end) {
      break;
    }

    for (int i = 0; i < n; ++i) {
      if (adjmatrix[current][i] && !visited[i]) {
        visited[i] = true;
        parent[i] = current;
        q.push(i);
        }
      }
  }
  // reconstruct the path
  vector<int> path;
  if (visited[end]) {
    for (int i = end; i != -1; i = parent[i]) {
      path.push_back(i);
    }
    reverse(path.begin(), path.end());
  }
  return path;
}

int main(int argc, char* argv[]) {
  vector<vector<int> > adjmatrix = import_graph(argv[1]);
  // there are (n choose 2) pairs in a graph with n vertices
  cout << "Path lengths: " << endl;
  map<pair<size_t,size_t>,size_t> path_lengths;
  map<pair<size_t,size_t>,size_t>::iterator it;
  for (size_t i = 0;i<adjmatrix.size();i++) {
    for (size_t j = i;j<adjmatrix.size();j++) {
      if (i == j) {
        continue;
      }
      vector<int> path = find_shortest_path(i, j, adjmatrix);
      printf("%d -> %d : ",(int)i,(int)j);
      for (int vertex : path) {
        cout << vertex << " ";
      }
      path_lengths.insert(make_pair(make_pair(i,j),path.size() - 1));
    }
  }
  cout << "Path lengths: " << '\n';
  for (size_t i = 0;i<adjmatrix.size();i++) {
    for (size_t j = 0;j<adjmatrix.size();j++) {
      if (i == j) {
        cout << "0 ";
        continue;
      }
      else if (j < i) {
        cout << "- ";
        continue;
      }
      else {
        cout << path_lengths.find(make_pair(i,j))->second << " ";
      }
    }
    cout << '\n';
  }
  return 0;
}
