#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

int main(int argc, char* argv[]) {
  stringstream ss;
  string file_name;
  if (argc != 2) {
    cerr << "usage: ./contract <file_name>" << '\n';
    return 1;
  }
  ss << argv[1];
  ss >> file_name;
  ifstream fin(file_name);
  if(!fin) {
    cerr << "Error opening file.\n";
    return 1;
  }

  int num_vertices, num_edges;
  fin >> num_vertices >> num_edges;

    // Define the graph type (undirected).
    // vecS for vertex container, vecS for edge container, undirectedS for an undirected graph.
    typedef boost::adjacency_list<
        boost::vecS,
        boost::vecS,
        boost::undirectedS
    > Graph;

    Graph g(num_vertices);

    // Read edges
    for(int i = 0; i < num_edges; i++) {
        int v1, v2;
        fin >> v1 >> v2;
        boost::add_edge(v1, v2, g);
    }

    fin.close();

    bool is_planar = boost::boyer_myrvold_planarity_test(g);

    if(is_planar) {
        cout << "The graph is planar.\n";
    } else {
        cout << "The graph is NOT planar.\n";
    }

    return 0;
}
