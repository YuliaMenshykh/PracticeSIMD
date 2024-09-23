#include <iostream>
#include <chrono>
#include <immintrin.h>  // For AVX2 intrinsics
#include <cstring>

// Loop-based substring searching function
int countSubstringLoop(const std::string& str, const std::string& substr) {
    int count = 0;
    size_t pos = 0;

    while ((pos = str.find(substr, pos)) != std::string::npos) {
        ++count;
        pos += substr.size();
    }

    return count;
}

// SIMD-based substring searching function using AVX2
int countSubstringSIMD(const std::string& str, const std::string& substr) {
    const char* largerStr = str.c_str();
    const char* subStr = substr.c_str();
    int strLen = str.length();
    int substrLen = substr.length();
    int count = 0;

    if (substrLen > 32) {
        std::cerr << "Substring too large for AVX2." << std::endl;
        return -1;
    }

    // Pad the substring to be 32 bytes using AVX2 (since AVX2 can work on 256 bits/32 bytes)
    char paddedSubStr[32] = { 0 };
    std::memcpy(paddedSubStr, subStr, substrLen);

    __m256i substrVec = _mm256_loadu_si256((__m256i*)paddedSubStr);

    for (int i = 0; i <= strLen - substrLen; i += 32) {
        __m256i strVec = _mm256_loadu_si256((__m256i*)(largerStr + i));
        __m256i cmpMask = _mm256_cmpeq_epi8(strVec, substrVec);
        int mask = _mm256_movemask_epi8(cmpMask);

        // Check if there's a full match in any byte
        if (mask != 0) {
            // Check each potential match for full equality
            for (int j = 0; j < 32; ++j) {
                if (i + j + substrLen <= strLen) {
                    if (std::strncmp(largerStr + i + j, subStr, substrLen) == 0) {
                        ++count;
                    }
                }
            }
        }
    }

    return count;
}

int main() {
    std::string largerString = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Lorem ipsum dolor sit amet.";
    std::string substring = "ipsum";

    // Measure time for loop-based search
    auto startLoop = std::chrono::high_resolution_clock::now();
    int loopResult = countSubstringLoop(largerString, substring);
    auto endLoop = std::chrono::high_resolution_clock::now();
    auto durationLoop = std::chrono::duration_cast<std::chrono::microseconds>(endLoop - startLoop).count();

    // Measure time for SIMD-based search
    auto startSIMD = std::chrono::high_resolution_clock::now();
    int simdResult = countSubstringSIMD(largerString, substring);
    auto endSIMD = std::chrono::high_resolution_clock::now();
    auto durationSIMD = std::chrono::duration_cast<std::chrono::microseconds>(endSIMD - startSIMD).count();

    // Output results
    std::cout << "Loop-based count: " << loopResult << " in " << durationLoop << " µs\n";
    std::cout << "SIMD-based count: " << simdResult << " in " << durationSIMD << " µs\n";

    return 0;
}
