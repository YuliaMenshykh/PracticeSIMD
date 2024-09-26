.data
    PUBLIC simdAddition
    PUBLIC loopAddition
   
.code

loopAddition PROC
    mov rax, rcx    ; data1 pointer
    mov rbx, rdx    ; data2 pointer
    mov rcx, r8     ; result pointer
    mov rdx, r9     ; size
    
    xor rsi, rsi    ; index
loop_start:
    cmp rsi, rdx
    jge loop_done
    mov r10, [rax + rsi*4]  ; load data1[i]
    add r10, [rbx + rsi*4]  ; add data2[i]
    mov [rcx + rsi*4],r10   ; store result[i]
    inc rsi              
    jmp loop_start
loop_done:
    ret
loopAddition ENDP




simdAddition PROC
    mov rax, rcx    ; data1 pointer
    mov rbx, rdx    ; data2 pointer
    mov rcx, r8     ; result pointer
    mov rdx, r9     ; size

    xor rsi, rsi    ; index

simdStart:
    cmp rsi, rdx
    jge simdDone

    vmovaps ymm0, YMMWORD PTR [rax + rsi*4]       
    vmovaps ymm1, YMMWORD PTR [rbx + rsi*4]
    vpaddd ymm0, ymm0, ymm1                       ; add packed integers
    vmovaps YMMWORD PTR [rcx + rsi*4], ymm0       ; store result

    add rsi, 8              
    jmp simdStart 

simdDone:
    ; Handle tail
    cmp rsi, rdx
    jge doneTail          ; if i >= size, skip scalar processing

tailStart:
    mov r10, [rax + rsi*4] 
    add r10, [rbx + rsi*4]
    mov [rcx + rsi*4], r10
    inc rsi               
    cmp rsi, rdx
    jl tailStart

doneTail:
    ret
simdAddition ENDP

END
