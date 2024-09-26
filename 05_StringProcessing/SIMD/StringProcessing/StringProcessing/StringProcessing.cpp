#include <iostream>
#include <chrono>
#include <immintrin.h> 
#include <cstring>

// Loop-based substring searching function
int countSubstringLoop(const std::string& str, const std::string& substr) 
{
    int count = 0;
    size_t pos = 0;

    while ((pos = str.find(substr, pos)) != std::string::npos) 
    {
        ++count;
        pos += substr.size();
    }

    return count;
}

int countSubstringSIMD(const std::string& str, const std::string& substr)
{
    const char* largerStr = str.c_str();
    const char* subStr = substr.c_str();
    int strLen = str.length();
    int substrLen = substr.length();
    int count = 0;

   
    // Pad the substring to be 32 bytes
    char paddedSubStr[32] = { 0 };
    std::memcpy(paddedSubStr, subStr, substrLen);

    __m256i substrVec = _mm256_loadu_si256((__m256i*)paddedSubStr);

    for (int i = 0; i <= strLen - substrLen; ++i)
    {
        // Load 32 bytes from the larger string at offset i
        __m256i strVec = _mm256_loadu_si256((__m256i*)(largerStr + i));

        // Compare the 32 bytes from the larger string with the substring
        __m256i cmpMask = _mm256_cmpeq_epi8(strVec, substrVec);
        int mask = _mm256_movemask_epi8(cmpMask);

        if (mask != 0)
        {
            // Check if there's a full match for the substring
            if (std::strncmp(largerStr + i, subStr, substrLen) == 0)
            {
                ++count;
                i += substrLen - 1;  // Skip ahead by the length of the substring
            }
        }
    }

    return count;
}

int main() 
{
    std::string largerString = "The instruction pointer register points to the next instruction to execute";
    std::string substring = "to";

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
