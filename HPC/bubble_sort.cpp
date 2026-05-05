#include <iostream>
#include <omp.h>
#include <iomanip> 
using namespace std;

int main() {
    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    int arr[n];

    cout << "Enter elements:\n";
    for(int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    double start, end;

    // ⏱️ Start time
    start = omp_get_wtime();

    // Parallel Bubble Sort (Odd-Even Transposition)
    for(int phase = 0; phase < n; phase++) {

        if(phase % 2 == 0) {
            #pragma omp parallel for
            for(int i = 0; i < n - 1; i += 2) {
                if(arr[i] > arr[i + 1]) {
                    swap(arr[i], arr[i + 1]);
                }
            }
        } else {
            #pragma omp parallel for
            for(int i = 1; i < n - 1; i += 2) {
                if(arr[i] > arr[i + 1]) {
                    swap(arr[i], arr[i + 1]);
                }
            }
        }
    }

    // ⏱️ End time
    end = omp_get_wtime();

    cout << "\nSorted array: ";
    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    cout << "\nExecution Time (Parallel): " << fixed << setprecision(9) << (end - start) << " seconds";

    return 0;
}