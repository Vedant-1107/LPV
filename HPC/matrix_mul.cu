%%writefile matrix_mul.cu
#include <iostream>
using namespace std;

#define N 2   // you can change size

// CUDA Kernel
__global__ void matrixMul(int *A, int *B, int *C, int n) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < n && col < n) {
        int sum = 0;
        for (int k = 0; k < n; k++) {
            sum += A[row * n + k] * B[k * n + col];
        }
        C[row * n + col] = sum;
    }
}

int main() {
    int n = N;
    int size = n * n * sizeof(int);

    // 🔹 Input matrices (custom values)
    int h_A[N][N] = { {3, 6}, {7, 5} };
    int h_B[N][N] = { {3, 5}, {6, 2} };
    int h_C[N][N];

    int *d_A, *d_B, *d_C;

    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    // Thread + block setup
    dim3 threads(2, 2);
    dim3 blocks(1, 1);

    matrixMul<<<blocks, threads>>>(d_A, d_B, d_C, n);

    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    // 🔹 Output format (like you want)
    cout << "Matrix A:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << h_A[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\nMatrix B:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << h_B[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\nMultiplication Result:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << h_C[i][j] << " ";
        }
        cout << endl;
    }

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}