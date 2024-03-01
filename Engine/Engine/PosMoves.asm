.DATA

.CODE

asmAdd PROC
push rbp
mov rbp, rsp

xor rbx, rbx
mov bl, byte ptr [rdx]
mov al, bl
imul rbx, 4
add rcx, rbx
mov r10b, byte ptr[rbp+48]
mov r11b, byte ptr[rbp+56]


mov byte ptr[rcx], r8b
inc rcx
mov byte ptr[rcx], r9b
inc rcx
mov byte ptr[rcx], r10b
inc rcx
mov byte ptr[rcx], r11b

inc al
mov byte ptr[rdx], al

pop rbp
ret
asmAdd ENDP

END