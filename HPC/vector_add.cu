#include <iostream>
using namespace std;

__global__ void vectorAdd(int *A, int *B, int *C, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n) {
        C[i] = A[i] + B[i];
    }
}

int main() {
    int n = 4;
    int size = n * sizeof(int);

    // Host arrays
    int h_A[] = {3, 6, 7, 5};
    int h_B[] = {3, 5, 6, 2};
    int h_C[4];

    // Device arrays
    int *d_A, *d_B, *d_C;

    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    // Copy to GPU
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    int blockSize = 256;
    int gridSize = (n + blockSize - 1) / blockSize;

    vectorAdd<<<gridSize, blockSize>>>(d_A, d_B, d_C, n);

    // Copy result back
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    // Output format
    cout << "Vector A: ";
    for (int i = 0; i < n; i++) cout << h_A[i] << " ";

    cout << "\nVector B: ";
    for (int i = 0; i < n; i++) cout << h_B[i] << " ";

    cout << "\nAddition: ";
    for (int i = 0; i < n; i++) cout << h_C[i] << " ";

    // Free memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}