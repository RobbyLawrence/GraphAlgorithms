#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <algorithm>
#include <set>

using namespace std;

struct SdEntry {
    int vertex;
    bool is_merged;
    int merged_with;
    vector<int> neighbors;

    SdEntry(int v, bool merged, int with, const vector<int>& n) : vertex(v), is_merged(merged), merged_with(with), neighbors(n) {}
};

class PlanarGraphFiveColorer {
    int n;
    vector<vector<bool>> adj_matrix;
    vector<list<int>> adjacency_lists;
    vector<bool> active;
    vector<int> degree;
    stack<int> S4, S5;
    stack<SdEntry> Sd;
    vector<int> color;

public:
    PlanarGraphFiveColorer(int numVertices) : n(numVertices), adj_matrix(numVertices, vector<bool>(numVertices, false)),
        adjacency_lists(numVertices), active(numVertices, true), degree(numVertices, 0), color(numVertices, -1) {}

    void addEdge(int u, int v) {
        if (!adj_matrix[u][v] && u != v) {
            adj_matrix[u][v] = true;
            adj_matrix[v][u] = true;
            adjacency_lists[u].push_back(v);
            adjacency_lists[v].push_back(u);
            degree[u]++;
            degree[v]++;
        }
    }

    void initializeStacks() {
        for (int v = 0; v < n; ++v) {
            if (degree[v] <= 4) {
                S4.push(v);
            } else if (degree[v] == 5) {
                bool hasLowDegreeNeighbor = false;
                for (int w : adjacency_lists[v]) {
                    if (degree[w] <= 6) {
                        hasLowDegreeNeighbor = true;
                        break;
                    }
                }
                if (hasLowDegreeNeighbor) {
                    S5.push(v);
                }
            }
        }
    }

    void processS4() {
        while (!S4.empty()) {
            int v = S4.top();
            S4.pop();
            if (!active[v]) continue;
            active[v] = false;
            vector<int> neighbors(adjacency_lists[v].begin(), adjacency_lists[v].end());
            Sd.emplace(v, false, -1, neighbors);
            for (int w : neighbors) {
                adjacency_lists[w].remove(v);
                degree[w]--;
                if (active[w] && degree[w] <= 4) {
                    S4.push(w);
                } else if (active[w] && degree[w] == 5) {
                    bool hasLow = false;
                    for (int u : adjacency_lists[w]) {
                        if (degree[u] <= 6) {
                            hasLow = true;
                            break;
                        }
                    }
                    if (hasLow) S5.push(w);
                }
            }
        }
    }

    void processS5() {
        while (!S5.empty()) {
            int v = S5.top();
            S5.pop();
            if (!active[v]) continue;
            int v1 = -1;
            for (int w : adjacency_lists[v]) {
                if (degree[w] <= 6) {
                    v1 = w;
                    break;
                }
            }
            if (v1 == -1) continue;
            auto it = find(adjacency_lists[v].begin(), adjacency_lists[v].end(), v1);
            int idx = distance(adjacency_lists[v].begin(), it);
            vector<int> neighbors;
            for (int i = 0; i < 5; ++i) {
                neighbors.push_back(*next(adjacency_lists[v].begin(), (idx + i) % 5));
            }
            int v3 = neighbors[2];
            if (!adj_matrix[v1][v3]) {
                active[v3] = false;
                Sd.emplace(v3, true, v1, vector<int>());
                list<int> toAdd;
                for (int w : adjacency_lists[v3]) {
                    if (w != v1 && !adj_matrix[v1][w]) {
                        toAdd.push_back(w);
                    }
                }
                for (int w : toAdd) {
                    adj_matrix[v1][w] = adj_matrix[w][v1] = true;
                    adjacency_lists[v1].push_back(w);
                    adjacency_lists[w].push_back(v1);
                    degree[v1]++;
                    degree[w]++;
                }
                for (int w : adjacency_lists[v3]) {
                    adjacency_lists[w].remove(v3);
                    if (active[w]) {
                        if (degree[w] <= 4) S4.push(w);
                        else if (degree[w] == 5) {
                            bool hasLow = false;
                            for (int u : adjacency_lists[w]) {
                                if (degree[u] <= 6) hasLow = true;
                            }
                            if (hasLow) S5.push(w);
                        }
                    }
                }
            } else {
                int v2 = neighbors[1], v4 = neighbors[3];
                active[v4] = false;
                Sd.emplace(v4, true, v2, vector<int>());
                list<int> toAdd;
                for (int w : adjacency_lists[v4]) {
                    if (w != v2 && !adj_matrix[v2][w]) {
                        toAdd.push_back(w);
                    }
                }
                for (int w : toAdd) {
                    adj_matrix[v2][w] = adj_matrix[w][v2] = true;
                    adjacency_lists[v2].push_back(w);
                    adjacency_lists[w].push_back(v2);
                    degree[v2]++;
                    degree[w]++;
                }
                for (int w : adjacency_lists[v4]) {
                    adjacency_lists[w].remove(v4);
                    if (active[w]) {
                        if (degree[w] <= 4) S4.push(w);
                        else if (degree[w] == 5) {
                            bool hasLow = false;
                            for (int u : adjacency_lists[w]) {
                                if (degree[u] <= 6) hasLow = true;
                            }
                            if (hasLow) S5.push(w);
                        }
                    }
                }
            }
            active[v] = false;
            vector<int> v_neighbors(adjacency_lists[v].begin(), adjacency_lists[v].end());
            Sd.emplace(v, false, -1, v_neighbors);
            for (int w : adjacency_lists[v]) {
                adjacency_lists[w].remove(v);
                degree[w]--;
                if (active[w] && degree[w] <= 4) S4.push(w);
                else if (active[w] && degree[w] == 5) {
                    bool hasLow = false;
                    for (int u : adjacency_lists[w]) {
                        if (degree[u] <= 6) hasLow = true;
                    }
                    if (hasLow) S5.push(w);
                }
            }
            break;
        }
    }

    void colorGraph() {
        while (!Sd.empty()) {
            SdEntry entry = Sd.top();
            Sd.pop();
            if (entry.is_merged) {
                color[entry.vertex] = color[entry.merged_with];
            } else {
                set<int> used;
                for (int w : entry.neighbors) {
                    if (color[w] != -1) used.insert(color[w]);
                }
                for (int c = 0; c < 5; ++c) {
                    if (used.find(c) == used.end()) {
                        color[entry.vertex] = c;
                        break;
                    }
                }
            }
        }
    }

    vector<int> getColors() {
        return color;
    }

    void fiveColor() {
        initializeStacks();
        while (true) {
            processS4();
            bool allInactive = all_of(active.begin(), active.end(), [](bool a) { return !a; });
            if (allInactive) break;
            processS5();
        }
        colorGraph();
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    PlanarGraphFiveColorer colorer(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        colorer.addEdge(u, v);
    }
    colorer.fiveColor();
    vector<int> colors = colorer.getColors();
    for (int c : colors) {
        cout << c << " ";
    }
    cout << endl;
    return 0;
}
