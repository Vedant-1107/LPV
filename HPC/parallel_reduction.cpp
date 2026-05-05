#include <iostream>
#include <omp.h>
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

    int sum = 0;
    int min_val = arr[0];
    int max_val = arr[0];

    // Parallel region
    #pragma omp parallel for reduction(+:sum) reduction(min:min_val) reduction(max:max_val)
    for(int i = 0; i < n; i++) {
        sum += arr[i];

        if(arr[i] < min_val)
            min_val = arr[i];

        if(arr[i] > max_val)
            max_val = arr[i];
    }

    float avg = (float)sum / n;

    cout << "\nSum = " << sum;
    cout << "\nMin = " << min_val;
    cout << "\nMax = " << max_val;
    cout << "\nAverage = " << avg;

    return 0;
}