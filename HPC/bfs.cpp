#include <iostream>
#include <omp.h>
#include <vector>
using namespace std;

int main() {
    int n;

    cout << "Enter number of vertices: ";
    cin >> n;

    int graph[n][n];

    cout << "Enter adjacency matrix:\n";
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cin >> graph[i][j];
        }
    }

    int start;
    cout << "Enter starting vertex: ";
    cin >> start;

    vector<int> visited(n, 0);
    vector<int> current_level, next_level;

    visited[start] = 1;
    current_level.push_back(start);

    double start_time = omp_get_wtime();

    while(!current_level.empty()) {

        next_level.clear();

        #pragma omp parallel for
        for(int i = 0; i < current_level.size(); i++) {
            int node = current_level[i];

            for(int j = 0; j < n; j++) {
                if(graph[node][j] == 1 && !visited[j]) {

                    #pragma omp critical
                    {
                        if(!visited[j]) {
                            visited[j] = 1;
                            next_level.push_back(j);
                        }
                    }
                }
            }
        }

        current_level = next_level;
    }

    double end_time = omp_get_wtime();

    cout << "\nBFS Traversal: ";
    for(int i = 0; i < n; i++) {
        if(visited[i])
            cout << i << " ";
    }

    cout << "\nExecution Time (Parallel): "
         << (end_time - start_time) << " seconds";

    return 0;
}


// Enter number of vertices: 4

// Adjacency Matrix:
// 0 1 1 0
// 1 0 1 1
// 1 1 0 0
// 0 1 0 0

// Start vertex: 0