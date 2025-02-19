#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <numeric>

using namespace std;

// using an adjacency list instead of matrix for this one
vector<vector<int> > read_graph(const string &file_name, int &num_vertices) {
  ifstream fin(file_name);
  if (!fin.is_open()) {
    cerr << "Unable to open file\n";
    exit(1);
  }
  int u, v;
  int max_vertex = -1;
  vector<pair<int, int> > edges;
  while (fin >> u >> v) {
    edges.push_back({u, v});
    if (u > max_vertex) {
      max_vertex = u;
    }
    if (v > max_vertex) {
      max_vertex = v;
    }
  }
  num_vertices = max_vertex + 1;
  vector<vector<int> > adj(num_vertices);
  for (pair<int,int> edge : edges) {
    u = edge.first;
    v = edge.second;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  return adj;
}
// create indices and order them by degree from high to low
vector<int> gen_high_low(const vector<int> &degrees) {
  int n = degrees.size();
  vector<int> order(n);
  // was using loops + .push_back(i) to fill vector, but it took to long so I switched to iota
  iota(order.begin(),order.end(),0);
  sort(order.begin(), order.end(), [&degrees](int a, int b) { // comparator function
    if (degrees[a] != degrees[b]) {
      return degrees[a] > degrees[b];
    } else {
        return a > b;
    }
  });
  return order;
}
// create indices and order them by degree from low to high
vector<int> gen_low_high(const vector<int> &degrees) {
  int n = degrees.size();
  vector<int> order(n);
  iota(order.begin(),order.end(),0);
  sort(order.begin(), order.end(), [&degrees](int a, int b) { // comparator function opposite of previous
    if (degrees[a] != degrees[b]) {
      return degrees[a] < degrees[b];
    }
    else {
      return a < b;
    }
  });

    return order;
}
// create indices and order them by degree randomly
vector<int> gen_random(int n) {
    vector<int> order(n);
    iota(order.begin(),order.end(),0);
    // had to look up how to use <random> lol
    random_device rd;
    mt19937 g(rd());
    shuffle(order.begin(), order.end(), g);
    return order;
}
// algorithm for greedy coloring
int greedy_coloring(const vector<vector<int> > &adj, const vector<int> &order) {
  int n = adj.size();
  vector<int> color(n, -1);
  int max_color = -1;
  vector<bool> used; // used vector to keep track of used colors
  for (int v : order) {
    if (max_color == -1) {
    used.assign(1,false);
    }
    else {
      used.assign(max_color + 2, false);
    }

    for (int neighbor : adj[v]) {
      if (color[neighbor] != -1) {
        if (color[neighbor] >= (int)used.size() ) {
          used.resize(color[neighbor] + 1, false);
        }
        used[color[neighbor]] = true;
      }
    }
    int c = 0;
    while (c < (int)used.size() && used[c]) {
      c++;
    }
    color[v] = c;
    if (c > max_color) {
      max_color = c;
    }
  }

  return max_color + 1;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <graph_file>" << endl;
    return 1;
  }
  int num_vertices;
  vector<vector<int> > adj = read_graph(argv[1], num_vertices);
  vector<int> degrees(num_vertices); // easy to construct
  for (int i = 0; i < num_vertices; ++i) {
    degrees[i] = adj[i].size();
  }

  // go through with greedy coloring using each ordering,
  // and record the time that it takes
  // I also had to look up how to use <chrono> lol
  auto start_high = chrono::high_resolution_clock::now();
  vector<int> order_high = gen_high_low(degrees);
  int colors_high = greedy_coloring(adj, order_high);
  auto end_high = chrono::high_resolution_clock::now();
  chrono::duration<double> elapsed_high = end_high - start_high;
  // low to high
  auto start_low = chrono::high_resolution_clock::now();
  vector<int> order_low = gen_low_high(degrees);
  int colors_low = greedy_coloring(adj, order_low);
  auto end_low = chrono::high_resolution_clock::now();
  chrono::duration<double> elapsed_low = end_low - start_low;
  // random
  auto start_random = chrono::high_resolution_clock::now();
  vector<int> order_random = gen_random(num_vertices);
  int colors_random = greedy_coloring(adj, order_random);
  auto end_random = chrono::high_resolution_clock::now();
  chrono::duration<double> elapsed_random = end_random - start_random;

  cout << "High to low: " << colors_high << " colors, " << fixed << elapsed_high.count() << " sec" << endl;
  cout << "Low to high: " << colors_low << " colors, " << fixed << elapsed_low.count() << " sec" << endl;
  cout << "Pseudo-random: " << colors_random << " colors, " << fixed << elapsed_random.count() << " sec" << endl;

  return 0;
}
