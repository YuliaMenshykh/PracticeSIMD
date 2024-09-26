#include <iostream>
#include <immintrin.h>
#include <chrono>

void MatrixPrint(const float* matrix, int numRows, int numCols, const char* label)
{
    std::cout << label << "\n";
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            float value = matrix[i * numCols + j];
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void simpleMatrixMul(const float* matrix1, const float* matrix2, float* result, int numRows, int numCols)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            float sum = 0.0f;
            for (int k = 0; k < numCols; k++)
            {
                float value1 = matrix1[i * numCols + k];
                float value2 = matrix2[k * numCols + j];
                sum += value1 * value2;
            }
            result[i * numCols + j] = sum;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Simple matrix mult took: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " ms\n";

    MatrixPrint(result, numRows, numCols, "Result (Matrix 3):");
}
void simdMatrixMul(const float* matrix1, const float* matrix2, float* result, int numRows, int numCols)
{
   
    matrix1 = std::assume_aligned<32>(matrix1);
    matrix2 = std::assume_aligned<32>(matrix2);
    result = std::assume_aligned<32>(result);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numRows; ++i)
    {
        for (int j = 0; j < numCols; ++j)
        {
            __m256 sum = _mm256_setzero_ps(); 

            for (int k = 0; k < numCols; k += 8)  // Process 8 elements at a time
            {
                // Load 8 elements from row i of matrix1
                __m256 vec1 = _mm256_load_ps(&matrix1[i * numCols + k]);

                // Manually load 8 elements from column j of matrix2 into a vector
                __m256 vec2 = _mm256_set_ps(
                    matrix2[(k + 7) * numCols + j],
                    matrix2[(k + 6) * numCols + j],
                    matrix2[(k + 5) * numCols + j],
                    matrix2[(k + 4) * numCols + j],
                    matrix2[(k + 3) * numCols + j],
                    matrix2[(k + 2) * numCols + j],
                    matrix2[(k + 1) * numCols + j],
                    matrix2[k * numCols + j]
                );

                sum = _mm256_fmadd_ps(vec1, vec2, sum);
            }

            // Reduce the 8 floats in sum into a single scalar value
            __m128 vhigh = _mm256_extractf128_ps(sum, 1);  // Extract high 128 bits (upper 4 floats)
            __m128 vresult = _mm_add_ps(_mm256_castps256_ps128(sum), vhigh);  // Add upper and lower 4 floats
            vresult = _mm_hadd_ps(vresult, vresult);  // Horizontal add
            vresult = _mm_hadd_ps(vresult, vresult);  // Final horizontal add

            // Store the result in the result matrix
            result[i * numCols + j] = _mm_cvtss_f32(vresult);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "SIMD matrix multiplication took: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds\n";

    MatrixPrint(result, numRows, numCols, "SIMD Result (Matrix 3):");
}

int main()
{
    const int numRows = 16;
    const int numCols = 16;

    // Dynamically allocate aligned memory for input matrices
    float* matrix1 = (float*)_mm_malloc(numRows * numCols * sizeof(float), 32);
    float* matrix2 = (float*)_mm_malloc(numRows * numCols * sizeof(float), 32);
    float* result = (float*)_mm_malloc(numRows * numCols * sizeof(float), 32);

    // Initialize the input matrices
    for (int i = 0; i < numRows * numCols; ++i) {
        matrix1[i] = i;
        matrix2[i] = i + 5;
    }

    
    MatrixPrint(matrix1, numRows, numCols, "Matrix 1:");
    MatrixPrint(matrix2, numRows, numCols, "Matrix 2:");
    


    // Perform simple matrix multiplication
    simpleMatrixMul(matrix1, matrix2, result, numRows, numCols);

    // Perform SIMD matrix multiplication
    simdMatrixMul(matrix1, matrix2, result, numRows, numCols);

    _mm_free(matrix1);
    _mm_free(matrix2);
    _mm_free(result);

    return 0;
}
