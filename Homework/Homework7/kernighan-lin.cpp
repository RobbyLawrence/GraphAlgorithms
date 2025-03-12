#include <iostream>
#include <vector>
#include <climits>
#include <fstream>

using namespace std;

int calc_cut_size(vector<bool> &in_A, vector<vector<int> > &adjmatrix) {
    int cut = 0;
    int n = (int)in_A.size();
    for (int u = 0; u < n; ++u) {
        if (in_A[u]) {
            for (int v = 0; v < n; ++v) {
                if (!in_A[v]) {
                    cut += adjmatrix[u][v];
                }
            }
        }
    }
    return cut;
}

void output(vector<bool> &in_A) {
    cout << "Partition A:";
    for (int i = 0; i < in_A.size(); ++i) if (in_A[i]) {
        cout << " " << i;
    }
    cout << "\nPartition B:";
    for (int i = 0; i < in_A.size(); ++i) if (!in_A[i]) {
        cout << " " << i;
    }
    cout << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "usage: ./kernighan-lin <file_name>" << endl;
        return 1;
    }
    ifstream fin(argv[1]);
    int n;
    cin >> n;
    vector<vector<int> > adjmatrix(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> adjmatrix[i][j];
        }
    }

    vector<bool> in_A(n, false);
    int half = (n + 1) / 2;
    for (int i = 0; i < half; ++i) {
        in_A[i] = true;
    }

    cout << "Initial partition:" << endl;
    output(in_A);
    int currentCut = calc_cut_size(in_A, adjmatrix);
    cout << "Initial cut size: " << currentCut << "\n" << endl;

    bool improved;
    int iteration = 0;
    do {
        ++iteration;
        improved = false;

        vector<int> original_A, original_B;
        for (int i = 0; i < n; ++i) {
            if (in_A[i]) original_A.push_back(i);
            else original_B.push_back(i);
        }

        vector<int> D(n, 0);
        for (int u = 0; u < n; ++u) {
            if (in_A[u]) {
                int external = 0, internal = 0;
                for (int v = 0; v < n; ++v) {
                    if (v == u) continue;
                    if (in_A[v]) internal += adjmatrix[u][v];
                    else external += adjmatrix[u][v];
                }
                D[u] = external - internal;
            } else {
                int external = 0, internal = 0;
                for (int v = 0; v < n; ++v) {
                    if (v == u) continue;
                    if (in_A[v]) external += adjmatrix[u][v];
                    else internal += adjmatrix[u][v];
                }
                D[u] = external - internal;
            }
        }

        vector<int> available_A = original_A;
        vector<int> available_B = original_B;
        vector<int> av, bv, gv;

        for (int step = 0; step < original_A.size(); ++step) {
            int max_g = INT_MIN;
            int best_a = -1, best_b = -1;

            for (int a : available_A) {
                for (int b : available_B) {
                    int g = D[a] + D[b] - 2 * adjmatrix[a][b];
                    if (g > max_g) {
                        max_g = g;
                        best_a = a;
                        best_b = b;
                    }
                }
            }

            if (best_a == -1 || best_b == -1) break;

            av.push_back(best_a);
            bv.push_back(best_b);
            gv.push_back(max_g);

            vector<int> new_available_A, new_available_B;
            for (int x : available_A) if (x != best_a) new_available_A.push_back(x);
            for (int y : available_B) if (y != best_b) new_available_B.push_back(y);

            for (int x : new_available_A) D[x] += adjmatrix[x][best_a];
            for (int y : new_available_B) D[y] += adjmatrix[y][best_b];

            available_A = new_available_A;
            available_B = new_available_B;
        }

        int max_sum = 0, current_sum = 0, best_k = 0;
        for (int k = 0; k < gv.size(); ++k) {
            current_sum += gv[k];
            if (current_sum > max_sum) {
                max_sum = current_sum;
                best_k = k + 1;
            }
        }

        if (max_sum > 0) {
            improved = true;
            for (int i = 0; i < best_k; ++i) {
                in_A[av[i]] = false;
                in_A[bv[i]] = true;
            }
        }

        currentCut = calc_cut_size(in_A, adjmatrix);
        cout << "Iteration " << iteration << ":" << endl;
        if (improved) {
            cout << " - Swapped " << best_k << " node pair(s)." << endl;
            cout << " - Cumulative gain: " << max_sum << endl;
        } else {
            cout << " - No swaps made. No improvement possible." << endl;
        }
        cout << " - Current cut size: " << currentCut << endl;
        output(in_A);
        cout << endl;
    } while (improved);

    cout << "Final partition:" << endl;
    output(in_A);
    cout << "Final cut size: " << currentCut << endl;

    return 0;
}
