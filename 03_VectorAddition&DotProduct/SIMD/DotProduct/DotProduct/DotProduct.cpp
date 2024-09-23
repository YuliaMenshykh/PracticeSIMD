#include <immintrin.h>
#include <iostream>
#include <chrono>

// SIMD vector addition
void simdVectorAdd(const float* vec1, const float* vec2, float* result, size_t size) 
{
    for (int i = 0; i < size; i += 8) {
        __m256 v1 = _mm256_load_ps(&vec1[i]); 
        __m256 v2 = _mm256_load_ps(&vec2[i]);
        __m256 res = _mm256_add_ps(v1, v2);  
        _mm256_store_ps(&result[i], res);     
    }
    
}

//SIMD dot product 
void simdDotProd( float* vec1, float* vec2, float& result, size_t size) {
    __m256 sum = _mm256_setzero_ps();
    vec1 = std::assume_aligned<32>(vec1);
    vec2 = std::assume_aligned<32>(vec2);

    for (size_t i = 0; i < size; i += 8) {
        __m256 v1 = _mm256_load_ps(&vec1[i]);
        __m256 v2 = _mm256_load_ps(&vec2[i]);
        //__m256 res = _mm256_mul_ps(v1, v2);
        sum = _mm256_fmadd_ps(v1,v2,sum);
    }

    // Horizontal add of all elements in the 256-bit register
    __m128 sum128 = _mm_add_ps(_mm256_castps256_ps128(sum), _mm256_extractf128_ps(sum, 1));
    sum128 = _mm_hadd_ps(sum128, sum128);
    sum128 = _mm_hadd_ps(sum128, sum128);

    _mm_store_ss(&result, sum128);

    //float temp[8];
    //_mm256_store_ps(temp, sum); // Store sum into an array
    //result = temp[0] + temp[1] + temp[2] + temp[3] + temp[4] + temp[5] + temp[6] + temp[7];
}


float simpleDotProduct(const float* vec1, const float* vec2, size_t size)
{
    float middleRes = 0.f;
    float result = 0.f;
    for (size_t i = 0; i < size; i++)
    {
        middleRes = vec1[i] * vec2[i];
        result += middleRes;
    }

    return result;
}

// Simple loop-based vector addition
void simpleVectorAdd(const float* vec1, const float* vec2, float* result, size_t size) 
{
    for (size_t i = 0; i < size; ++i) 
    {
        result[i] = vec1[i] + vec2[i];
    }
}

int main() {
    const size_t size = 1024; 

    float* vector1 = (float*)_mm_malloc(size * sizeof(float), 32);
    float* vector2 = (float*)_mm_malloc(size * sizeof(float), 32);
    float* resultSimd = (float*)_mm_malloc(size * sizeof(float), 32);
    float* resultSimple = (float*)_mm_malloc(size * sizeof(float), 32);

    float simdDotResult = 0.f;
    float simpleDotResult =0.f;

    // Initialize vectors
    for (int i = 0; i < size; ++i) {
        vector1[i] = i;
        vector2[i] = i+5.f;
    }
    
    std::cout << "\n";

    // Measure performance of SIND version
    auto start = std::chrono::high_resolution_clock::now();

    simdVectorAdd(vector1, vector2, resultSimd, size);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "SIMD Vector Addition took: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";
    for (size_t i = 0; i < 15; ++i) 
    {
        std::cout << resultSimd[i] << " ";
    }
    std::cout << "\n";

    // Measure performance of simple version
    start = std::chrono::high_resolution_clock::now();
    simpleVectorAdd(vector1, vector2, resultSimple, size);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Simple Vector Addition took: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";

    // Dot product measure performance (SIMD)
    start = std::chrono::high_resolution_clock::now();
    simdDotProd(vector1, vector2, simdDotResult, size);
    std::cout << "SIMD dot product result: " << simdDotResult<< std::endl;

    end = std::chrono::high_resolution_clock::now();
    std::cout << "SIMD dot product took: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";



    // Dot product measure performance (loop based)
    start = std::chrono::high_resolution_clock::now();
    simpleDotResult = simpleDotProduct(vector1, vector2, size);
    std::cout << "Simple dot product result: " << simpleDotResult << std::endl;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Simple dot product took: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";



    // Cleanup
    _mm_free(vector1);
    _mm_free(vector2);
    _mm_free(resultSimd);
    _mm_free(resultSimple);

    return 0;
}
