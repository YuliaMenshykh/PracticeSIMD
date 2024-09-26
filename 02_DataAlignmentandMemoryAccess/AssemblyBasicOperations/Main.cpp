#include <iostream>
#include <chrono>
#include <immintrin.h> 

extern "C" void loopAddition(const int* data1, const int* data2, int* result, int size);
extern "C" void simdAddition(const int* data1, const int* data2, int* result, int size);

int main()
{
    const int size = 10000000;
    int* data1 = (int*)_mm_malloc(size * sizeof(int), 32); 
    int* data2 = (int*)_mm_malloc(size * sizeof(int), 32);
    int* result = (int*)_mm_malloc(size * sizeof(int), 32);

    // Initialize the data
    for (int i = 0; i < size; ++i) {
        data1[i] = i;
        data2[i] = i + 5;
    }
  

    // Loop-based addition
    auto start = std::chrono::high_resolution_clock::now();
    loopAddition(data1, data2, result, size);

    auto end = std::chrono::high_resolution_clock::now();
    auto loopDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Loop addition took: " << loopDuration << " ms" << std::endl;
    std::cout << "First 15 results from loop-based addition: ";
    for (int i = 0; i < 15; ++i) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;



    // SIMD-based addition (timing)
    start = std::chrono::high_resolution_clock::now();
    simdAddition(data1, data2, result, size);

    end = std::chrono::high_resolution_clock::now();
    auto simdDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "SIMD addition took: " << simdDuration << " ms" << std::endl;
    std::cout << "First 15 results from SIMD addition: ";
    for (int i = 0; i < 15; ++i) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

    // Free aligned memory
    _mm_free(data1);
    _mm_free(data2);
    _mm_free(result);

    return 0;
}
