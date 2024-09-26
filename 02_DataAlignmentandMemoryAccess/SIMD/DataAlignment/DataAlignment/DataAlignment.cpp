#include "immintrin.h" 
#include <chrono>
#include <iostream>

void simpleAddition(const int* data1, const int* data2, int* result, int arraySize)
{
	for (int i = 0; i < arraySize; i++)
	{
		result[i] = data1[i] + data2[i];
	}
}
void simdAddition(const int* data1, const int* data2, int* result, int arraySize)
{
	int simdWidth = 8;
	int i = 0;

	// Ensure data is aligned to 32 bytes
	data1 = (int*)std::assume_aligned<32>(data1);
	data2 = (int*)std::assume_aligned<32>(data2);
	result = (int*)std::assume_aligned<32>(result);

	// Process in chunks of 8 integers
	for (i = 0; i + simdWidth <= arraySize; i += simdWidth)
	{
		__m256i vec1 = _mm256_load_si256((__m256i*) & data1[i]);
		__m256i vec2 = _mm256_load_si256((__m256i*) & data2[i]);

		__m256i vecResult = _mm256_add_epi32(vec1, vec2);
		_mm256_store_si256((__m256i*) & result[i], vecResult);
	}

	// Process remaining elements that do not fit in registers
	for (; i < arraySize; i++)
	{
		result[i] = data1[i] + data2[i];
	}
}

int main()
{
	const int size = 10000000; 
	int* data1 = (int*)_mm_malloc(size * sizeof(int), 32);
	int* data2 = (int*)_mm_malloc(size * sizeof(int), 32);
	int* result = (int*)_mm_malloc(size * sizeof(int), 32);

	for (int i = 0; i < size; ++i) {
		data1[i] = i;
		data2[i] = i + 5;
	}

	//Loop based addition
	auto start_loop = std::chrono::high_resolution_clock::now();
	simpleAddition(data1, data2, result, size);

	auto end_loop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_loop - start_loop);
	std::cout << "Loop addition took : ";
	std::cout << duration.count() << " ms" << std::endl;
	// Print first 15 results
	std::cout << "Result of loop based addition (first 15 results): " << '\n' 
		<< "Printing only the first 15 results because printing all 10 000 000 results would take too long." << '\n';
	for (int i = 0; i < 15; ++i) {
		std::cout << result[i] << " ";  
	}
	std::cout <<"\n" << std::endl;


	//SIMD addition
	start_loop = std::chrono::high_resolution_clock::now();
	simdAddition(data1, data2, result, size);

	end_loop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_loop - start_loop);
	std::cout << "SIMD addition took : ";
	std::cout << duration.count() << " ms" << std::endl;

	std::cout << "Result of SIMD addition (first 15 results): ";
	// Print first 15 results
	for (int i = 0; i < 15; ++i) {
		std::cout << result[i] << " ";
	}
	std::cout << std::endl;



	_mm_free(data1);
	_mm_free(data2);
	_mm_free(result);
	return 0;
}