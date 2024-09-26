.data
    array1 DWORD 34, 2, 11, 9, 1, 62, 51, 16
    array2 DWORD 5, 72, 33, 7, 14, 8, 2, 77
    resultArray DWORD 8 DUP(0)

    simdTime QWORD ?   ; Store 64-bit timing results
    loopTime QWORD ?

    PUBLIC simdAddition
    PUBLIC loopAddition
    PUBLIC getResultArray

.code

simdAddition PROC
    vmovdqu ymm0, YMMWORD PTR array1
    vmovdqu ymm1, YMMWORD PTR array2
    vpaddd ymm0, ymm0, ymm1
    vmovdqu YMMWORD PTR resultArray, ymm0

    ret
simdAddition ENDP


loopAddition PROC
    mov rcx, 8          ; loop counter
    lea rsi, [array1]        
    lea rdi, [array2]        
    lea rdx, [resultArray]  
    
loopStart:
    mov rbx, [rsi]           
    add rbx, [rdi]     
    mov [rdx], rbx

    add rsi, 4      ; Move to next element
    add rdi, 4
    add rdx, 4
    loop loopStart

    ret
loopAddition ENDP

getResultArray PROC
    mov rax, OFFSET resultArray
    ret
getResultArray ENDP

END