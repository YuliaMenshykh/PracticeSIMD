#include "immintrin.h" 
#include <chrono>
#include <iostream>


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

void simdAdd(int* simdResult, const int* arr1, const int* arr2)
{
    __m256i result;
    __m256i vector1 = _mm256_load_si256((__m256i*)arr1); //doing it here because arraySize = 8
    __m256i vector2 = _mm256_load_si256((__m256i*)arr2);
    for (int j = 0; j < 1000000; ++j)
    {
        result = _mm256_add_epi32(vector1, vector2);
    }
    _mm256_storeu_si256((__m256i*)simdResult, result);
}
   

int main()
{
    const int arraySize = 8;
    alignas(32) int arr1[arraySize], arr2[arraySize], loopResult[arraySize], simdResult[arraySize];;
    
    // Initialize arrays with values
    for (int i = 0; i < arraySize; i++) 
    {
        arr1[i] = i;
        arr2[i] = i * 2;
    }



	//SIMD execution
    //Measure SIMD addition
    auto start = std::chrono::high_resolution_clock::now();
    simdAdd(simdResult, arr1, arr2);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "SIMD addition took " <<  std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    //Print SIMD addition result
    std::cout << '\n' << "Result of SIMD addition: ";
    for (int i = 0; i < arraySize; ++i) 
    {
        std::cout << simdResult[i] << " ";  
    }
    std::cout << std::endl;



    // Loop based
    // Measure simple loop based execution 
    start = std::chrono::high_resolution_clock::now();
    loopAddition(arr1, arr2, loopResult, arraySize);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Loop addition took : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()<< std::endl;
    
    //Print loop based addition result
    std::cout << "Result of loop based addition: ";
    for (int i = 0; i < arraySize; ++i) 
    {
        std::cout << loopResult[i] << " ";  
    }
    std::cout << std::endl;

	return 0;
}



