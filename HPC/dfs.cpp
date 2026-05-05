#include <iostream>
#include <omp.h>
#include <iomanip>
using namespace std;

int n;
int graph[100][100];
bool visited[100];

// Parallel DFS function
void parallelDFS(int node) {

    bool shouldProcess = false;

    // Critical section to safely check & mark visited
    #pragma omp critical
    {
        if (!visited[node]) {
            visited[node] = true;
            cout << node << " ";
            shouldProcess = true;
        }
    }

    // If already visited, exit safely (outside OpenMP block)
    if (!shouldProcess)
        return;

    // Explore neighbors in parallel
    for (int i = 0; i < n; i++) {
        if (graph[node][i] == 1) {
            #pragma omp task
            parallelDFS(i);
        }
    }

    // Wait for all child tasks to finish
    #pragma omp taskwait
}

int main() {
    cout << "Enter number of vertices: ";
    cin >> n;

    cout << "Enter adjacency matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> graph[i][j];
        }
    }

    int start;
    cout << "Enter starting vertex: ";
    cin >> start;

    // Initialize visited array
    for (int i = 0; i < n; i++)
        visited[i] = false;

    double start_time = omp_get_wtime();

    // Start parallel region
    #pragma omp parallel
    {
        #pragma omp single
        {
            parallelDFS(start);
        }
    }

    double end_time = omp_get_wtime();

    cout << "\nExecution Time (Parallel): "
         << fixed << setprecision(9)
         << (end_time - start_time) << " seconds";

    return 0;
}