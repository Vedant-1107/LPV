#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Merge Function
void merge(vector<int>& arr, int l, int m, int r) {

    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);

    for(int i = 0; i < n1; i++)
        L[i] = arr[l + i];

    for(int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0;
    int j = 0;
    int k = l;

    while(i < n1 && j < n2) {

        if(L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while(i < n1)
        arr[k++] = L[i++];

    while(j < n2)
        arr[k++] = R[j++];
}

// Sequential Merge Sort
void sequential_merge_sort(vector<int>& arr, int l, int r) {

    if(l < r) {

        int m = l + (r - l) / 2;

        sequential_merge_sort(arr, l, m);
        sequential_merge_sort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

// Parallel Merge Sort Utility
void parallel_merge_sort_util(vector<int>& arr, int l, int r) {

    if(l < r) {

        int m = l + (r - l) / 2;

        // Create task for left half
        #pragma omp task shared(arr)
        {
            parallel_merge_sort_util(arr, l, m);
        }

        // Create task for right half
        #pragma omp task shared(arr)
        {
            parallel_merge_sort_util(arr, m + 1, r);
        }

        // Wait for tasks
        #pragma omp taskwait

        // Merge
        merge(arr, l, m, r);
    }
}

// Parallel Merge Sort
void parallel_merge_sort(vector<int>& arr) {

    #pragma omp parallel
    {
        #pragma omp single
        {
            parallel_merge_sort_util(arr, 0, arr.size() - 1);
        }
    }
}

// Print Array
void print_array(vector<int>& arr) {

    for(int x : arr)
        cout << x << " ";

    cout << endl;
}

int main() {

    vector<int> arr1 = {
        55, 12, 78, 34, 23,
        90, 11, 67, 45, 89,
        21, 43, 65, 10, 99,
        76, 32, 54, 87, 1
    };

    vector<int> arr2 = arr1;

    double start, end;

    // Sequential Merge Sort
    start = omp_get_wtime();

    sequential_merge_sort(arr1, 0, arr1.size() - 1);

    end = omp_get_wtime();

    cout << "Sequential Merge Sort Time: "
         << end - start << " seconds\n";

    cout << "Sequential Sorted Array:\n";

    print_array(arr1);

    cout << endl;

    // Parallel Merge Sort
    start = omp_get_wtime();

    parallel_merge_sort(arr2);

    end = omp_get_wtime();

    cout << "Parallel Merge Sort Time: "
         << end - start << " seconds\n";

    cout << "Parallel Sorted Array:\n";

    print_array(arr2);

    return 0;
}
