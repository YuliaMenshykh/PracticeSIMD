#include "immintrin.h" 
#include <chrono>
#include <iostream>
#include "BasicOperations.h"




void loopAddition(const int* arr1, const int* arr2, int* result, int arraySize)
{
    for (int j = 0; j < 1000000; ++j)
    {
        for (int i = 0; i < arraySize; i++) 
        {
            result[i] = arr1[i] + arr2[i];
        }
    }
}

void simdAdd(__m256i& tempResult, const __m256i& vector1, const __m256i& vector2)
{
    for (int j = 0; j < 1000000; ++j)
    {
        tempResult = _mm256_add_epi32(vector1, vector2);
    }
    
}

int main()
{
    const int arraySize = 8;
    // Data definition
    int arr1[arraySize], arr2[arraySize], loopResult[arraySize];
    __m256i simdResult;
    
    // Initialize arrays with values
    for (int i = 0; i < arraySize; i++) {
        arr1[i] = i;
        arr2[i] = i * 2;
    }

	//SIMD execution
    __m256i vector1 = _mm256_loadu_si256((__m256i*)arr1);
    __m256i vector2 = _mm256_loadu_si256((__m256i*)arr2);
    __m256i tempResult;
    auto start = std::chrono::high_resolution_clock::now();
    simdAdd(tempResult, vector1, vector2);auto end = std::chrono::high_resolution_clock::now();
    std::cout << "SIMD addition took " <<  std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();


    int result[arraySize];
    _mm256_storeu_si256((__m256i*)result, tempResult);  // Store the result in a regular array

    std::cout << '\n' << "Result of SIMD addition: ";
    for (int i = 0; i < arraySize; ++i) {
        std::cout << result[i] << " ";  // Print the result
    }
    std::cout << std::endl;

    // Loop based
    // Measure execution 
    auto start_loop = std::chrono::high_resolution_clock::now();

    loopAddition(arr1, arr2, loopResult, arraySize);

    auto end_loop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_loop - start_loop);
    std::cout <<"Loop addition took : ";
    std::cout << duration.count() << " ms" << std::endl;

    std::cout << "Result of loop based addition: ";
    for (int i = 0; i < arraySize; ++i) {
        std::cout << loopResult[i] << " ";  // Print the result
    }
    std::cout << std::endl;

	return 0;
}



