.data
    array1 DWORD 34, 2, 11, 9, 1, 62, 51, 16
    array2 DWORD 5, 72, 33, 7, 14, 8, 2, 77
    resultArray DWORD 8 DUP(0)

    simdTime QWORD ?   ; Store 64-bit timing results
    loopTime QWORD ?

    PUBLIC simd_addition
    PUBLIC loop_addition
    PUBLIC get_result_array
    PUBLIC simdTime
    PUBLIC loopTime

.code
simd_addition PROC
    ; Measure SIMD Addition Performance
    rdtsc
    mov rbx, rax ; Save start time

    ; SIMD Addition using AVX2
    vmovdqu ymm0, YMMWORD PTR array1
    vmovdqu ymm1, YMMWORD PTR array2

    vpaddd ymm0, ymm0, ymm1

    vmovdqu YMMWORD PTR resultArray, ymm0

    ; Measure end time for SIMD
    rdtsc
    sub rax, rbx
    mov simdTime, rax
    ret
simd_addition ENDP

loop_addition PROC
    ; Measure Loop-based Addition Performance
    rdtsc
    mov rbx, rax ; Save start time (use rbx for 64-bit registers)

    mov rcx, 8               ; Number of elements in arrays (use rcx for 64-bit loop counter)
    lea rsi, [array1]        ; Load address of array1 into rsi
    lea rdi, [array2]        ; Load address of array2 into rdi
    lea rdx, [resultArray]   ; Load address of resultArray into rdx

loop_start:
    mov eax, [rsi]           ; Load element from array1
    add eax, [rdi]           ; Add element from array2
    mov [rdx], eax           ; Store result in resultArray

    add rsi, 4               ; Move to next element (4 bytes for DWORD)
    add rdi, 4
    add rdx, 4
    loop loop_start

    ; Measure end time for loop
    rdtsc
    sub rax, rbx
    mov loopTime, rax        ; Store time in 64-bit variable
    ret
loop_addition ENDP

get_result_array PROC
    mov rax, OFFSET resultArray
    ret
get_result_array ENDP

END