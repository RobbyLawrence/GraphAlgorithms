#include <iostream>
#include <vector>
using namespace std;

struct Graph {
  vector<vector<int> > adjmatrix;
  int num_edges = 0;
  int num_vertices = 0;
  Graph(int vertices, int edges) {
    num_edges = edges;
    num_vertices = vertices;
  }
}

bool planarity(Graph G){
    int E = 0;
    for(auto v : G){
        E += v.size();
    }
    E = E/2;
    if(E > 3*V - 3){
        return false;
    }
    vector<graph> BC;
    biconnectedComponents(G, BC);

    for(auto C : BC){
        palm P = transform(C);
        cycle c = getCycle(P);
        planar plane = constructPlanar(c);
        vector<graph> pieces = delCycle(c, plane);
        for(auto piece : pieces){
            if(planarity(piece)){
                addPiece(piece);
            }
            else{
                return false;
            }
        }
    }
}

int main() {
  return 0;
}
