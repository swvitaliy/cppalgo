#include <cuda.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>

#define MAX_SIZE 10

__global__ void radix_sort_kernel(int* A, int n) {
  // Find the maximum value
  int max_val = 0;
  for (int i = 0; i < n; i++) max_val = max(max_val, A[i]);

  // Find the number of digits in the maximum value
  int digits = 0;
  while (max_val > 0) {
    digits++;
    max_val /= 10;
  }

  // Create an array to store counts for each digit
  int counts[10];
  for (int i = 0; i < 10; i++) counts[i] = 0;

  // Count the number of occurrences of each digit
  for (int i = 0; i < n; i++) {
    int digit = A[i] % 10;
    counts[digit]++;
  }

  // Calculate the starting index for each digit
  int start[10];
  start[0] = 0;
  for (int i = 1; i < 10; i++) start[i] = start[i - 1] + counts[i - 1];

  // Create an array to store the sorted list
  int B[n];

  // Sort the elements
  for (int d = 0; d < digits; d++) {
    // Copy the elements into the temp array
    for (int i = 0; i < n; i++) {
      int digit = (A[i] / (int)pow(10, d)) % 10;
      int index = start[digit]++;
      B[index] = A[i];
    }

    // Copy the elements back into the original array
    for (int i = 0; i < n; i++) A[i] = B[i];
  }
}

int main() {
  int arr[MAX_SIZE];
  int* dev_arr;

  // Fill array with random values
  for (int i = 0; i < MAX_SIZE; i++) arr[i] = rand() % 100;

  // Allocate memory on the device
  cudaMalloc((void**)&dev_arr, MAX_SIZE * sizeof(int));

  // Copy array to the device
  cudaMemcpy(dev_arr, arr, MAX_SIZE * sizeof(int), cudaMemcpyHostToDevice);

  // Run the kernel
  radix_sort_kernel<<<1, MAX_SIZE>>>(dev_arr, MAX_SIZE);

  // Copy the results back to the host
  cudaMemcpy(arr, dev_arr, MAX_SIZE * sizeof(int), cudaMemcpyDeviceToHost);

  // Print the results
  for (int i = 0; i < MAX_SIZE; i++) std::cout << arr[i] << " ";

  std::cout << std::endl;

  // Free device memory
  cudaFree(dev_arr);

  return 0;
}