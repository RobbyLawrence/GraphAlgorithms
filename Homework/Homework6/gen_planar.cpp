#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <cmath>
#include <tuple>
#include <fstream>

using namespace std;

int main() {
    int num_vertices;
    double density;
    ofstream fout("testing.dim");
    cout << "Enter number of vertices: ";
    cin >> num_vertices;
    cout << "Enter density: ";
    cin >> density;
    if (num_vertices < 1) {
        cerr << "Number of vertices must be at least 1." << endl;
        return 1;
    }

    int max_possible_edges;
    if (num_vertices >= 3) {
        max_possible_edges = 3 * num_vertices - 6;
    } else if (num_vertices == 2) {
        max_possible_edges = 1;
    } else {
        max_possible_edges = 0;
    }

    int desired_edges = round(density * max_possible_edges);
    desired_edges = max(0, min(desired_edges, max_possible_edges));

    set<pair<int, int>> edges;

    if (num_vertices >= 3) {
        // Initial triangle (0, 1, 2)
        edges.insert({0, 1});
        edges.insert({1, 2});
        edges.insert({2, 0});

        vector<tuple<int, int, int>> face_list;
        face_list.emplace_back(0, 1, 2);

        random_device rd;
        mt19937 gen(rd());

        for (int v = 3; v < num_vertices; ++v) {
            // Randomly select a face
            uniform_int_distribution<> dis(0, face_list.size() - 1);
            int selected_face_idx = dis(gen);
            auto face = face_list[selected_face_idx];
            int a = get<0>(face), b = get<1>(face), c = get<2>(face);

            // Add edges from the new vertex to the selected face's vertices
            edges.insert({min(v, a), max(v, a)});
            edges.insert({min(v, b), max(v, b)});
            edges.insert({min(v, c), max(v, c)});

            // Remove the selected face and add new faces
            face_list.erase(face_list.begin() + selected_face_idx);
            face_list.emplace_back(v, a, b);
            face_list.emplace_back(v, b, c);
            face_list.emplace_back(v, c, a);
        }
    } else if (num_vertices == 2) {
        edges.insert({0, 1});
    }

    // Adjust edges to meet desired density
    int current_edges = edges.size();
    if (desired_edges < current_edges) {
        vector<pair<int, int>> edge_vector(edges.begin(), edges.end());
        shuffle(edge_vector.begin(), edge_vector.end(), mt19937(random_device{}()));
        edge_vector.resize(desired_edges);
        edges = set<pair<int, int>>(edge_vector.begin(), edge_vector.end());
    }

    // Output the result
    fout << num_vertices << " " << edges.size() << endl;
    for (const auto& edge : edges) {
        fout << edge.first << " " << edge.second << endl;
    }

    return 0;
}
