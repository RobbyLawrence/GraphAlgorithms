#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <numeric>

using namespace std;


vector<vector<int> > read_graph(const string &file_name, int &num_vertices) {
    ifstream fin(file_name);
    if (!fin.is_open()) {
        cerr << "Unable to open file\n";
        exit(1);
    }
    int num_edges;
    fin >> num_vertices >> num_edges;

    vector<vector<int> > adj(num_vertices);
    int u, v;
    for (int i = 0; i < num_edges; ++i) {
        fin >> u >> v;
        if (u >= num_vertices || v >= num_vertices) {
            cerr << "vertex number is greater than number of vertices\n";
            exit(1);
        }
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    return adj;
}

vector<int> gen_high_low(vector<int> &degrees) { // taken from my greedy coloring algorithm
    int n = degrees.size();
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&degrees](int a, int b) {
        return degrees[a] > degrees[b] || (degrees[a] == degrees[b] && a > b);
    });
    return order;
}

vector<int> bfs_order(vector<vector<int> > &adj, int start) {
    int n = adj.size();
    vector<int> order;
    vector<bool> visited(n, false);
    queue<int> q;

    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            q.push(i);
            visited[i] = true;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                order.push_back(u);
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }
    }

    return order;
}

vector<int> greedy_coloring(vector<vector<int> > &adj, vector<int> &order) { // also stolen from greedy coloring
    int n = adj.size();
    vector<int> color(n, -1);
    int max_color = -1;
    vector<bool> used;

    for (int v : order) {
        if (max_color == -1) {
            used.assign(1, false);
        } else {
            used.assign(max_color + 2, false);
        }

        for (int neighbor : adj[v]) {
            if (color[neighbor] != -1) {
                if (color[neighbor] >= (int)used.size()) {
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

    return color;
}

void output_colors(vector<int> &colors) {
    for (size_t i = 0; i < colors.size(); ++i) {
        cout << i << " " << colors[i] << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./planar_color <file_name>" << endl;
        return 1;
    }

    int num_vertices;
    vector<vector<int> > adj = read_graph(argv[1], num_vertices);
    vector<int> degrees(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
        degrees[i] = adj[i].size();
    }

    vector<int> order;
    vector<int> colors;

    order = gen_high_low(degrees);
    colors = greedy_coloring(adj, order);
    if (*max_element(colors.begin(), colors.end()) + 1 <= 5) {
        output_colors(colors);
        return 0;
    }
    for (int start = 0; start < num_vertices; ++start) {
        order = bfs_order(adj, start);
        colors = greedy_coloring(adj, order);
        if (*max_element(colors.begin(), colors.end()) + 1 <= 5) {
            output_colors(colors);
            return 0;
        }
    }
    cerr << "No 5-coloring found." << endl;
    return 1;
}
