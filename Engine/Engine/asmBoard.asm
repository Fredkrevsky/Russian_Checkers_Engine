.DATA

InitB     BYTE  1, 0, 1, 0, 0, 0, 2, 0
          BYTE  0, 1, 0, 0, 0, 2, 0, 2 
          BYTE  1, 0, 1, 0, 0, 0, 2, 0 
     	  BYTE  0, 1, 0, 0, 0, 2, 0, 2 
          BYTE  1, 0, 1, 0, 0, 0, 2, 0
     	  BYTE  0, 1, 0, 0, 0, 2, 0, 2 
    	  BYTE  1, 0, 1, 0, 0, 0, 2, 0 
    	  BYTE  0, 1, 0, 0, 0, 2, 0, 2

AssessD   BYTE  10, 0,  8,  0,  8,  0,  8,  0
		  BYTE  0,  10, 0,  6,  0,  6,  0,  8
		  BYTE  8,  0,  10, 0,  6,  0,  6,  0
		  BYTE  0,  6,  0,  10, 0,  6,  0,  8
		  BYTE  8,  0,  6,  0,  10, 0,  6,  0 
		  BYTE  0,  6,  0,  6,  0,  10, 0,  8
		  BYTE  8,  0,  6,  0,  6,  0,  10, 0 
		  BYTE  0,  8,  0,  8,  0,  8,  0,  10

AssessS   BYTE  8,  0,  9,  0,  9,  0,  12, 0
		  BYTE  0,  8,  0,  9,  0,  10, 0,  10
		  BYTE  8,  0,  10, 0,  11, 0,  14, 0
		  BYTE  0,  9,  0,  11, 0,  12, 0,  10
	  	  BYTE  12, 0,  10, 0,  11, 0,  14, 0
		  BYTE  0,  9,  0,  11, 0,  12, 0,  10
		  BYTE  12, 0,  9,  0,  9,  0,  14, 0
		  BYTE  0,  8,  0,  9,  0,  10, 0,  10

.CODE

CheckCoord PROC
	mov rax, 0
	
	cmp cl, 0
	jl Ending
	cmp cl, 7
	jg Ending

	cmp dl, 0
	jl Ending
	cmp dl, 7
	jg Ending

	mov rax, 1

Ending:

	ret
CheckCoord ENDP


CheckCoord4 PROC
push rbp
mov rbp, rsp

xor rax, rax

cmp cl, 0
jl Ending
cmp cl, 7
jg Ending

cmp dl, 0
jl Ending
cmp dl, 7
jg Ending

cmp r8b, 0
jl Ending
cmp r8b, 7
jg Ending

cmp r9b, 0
jl Ending
cmp r9b, 7
jg Ending

mov rax, 1

Ending:

pop rbp
ret
CheckCoord4 ENDP


GetMode PROC
	push rbp
	mov rbp, rsp

	movsx rax, byte ptr[rbp + 48]

	test rax, rax
	jz Mode0

	cmp rax, 2
	je Mode23

	cmp rax, 3
	je Mode23

	jmp Mode14

Mode0:
	
	cmp rdx, r9
	jng m0y1notBiggery2

		cmp rcx, r8
		jng m0x1notBiggerx2
		mov rax, 4
		jmp Ending

	m0x1notBiggerx2:

		mov rax, 3
		jmp Ending

m0y1notBiggery2:
		
		cmp rcx, r8
		jng m02x1notBiggerx2
		mov rax, 2
		jmp Ending

	m02x1notBiggerx2:

		mov rax, 1
		jmp Ending

Mode14:
	sub rcx, rdx
	sub r8, r9
	cmp rcx, r8
	jg m14Bigger
	jl m14Lower
	jmp Ending

	m14Bigger:
		mov rax, 2
		jmp Ending
	m14Lower:
		mov rax, 3
		jmp Ending
Mode23:
	add rcx, rdx
	add r8, r9
	cmp rcx, r8
	jg m23Bigger
	jl m23Lower
	jmp Ending

	m23Bigger:
		mov rax, 4
		jmp Ending
	m23Lower:
		mov rax, 1

Ending:
	
	pop rbp
	ret
GetMode ENDP

;---------------------------------------------- BInit 

BInit PROC

mov rdi, rcx
lea rsi, OFFSET InitB

mov rcx, 8
rep movsq

ret
BInit ENDP

;---------------------------------------------- BInit END


;---------------------------------------------- BCopy 

BCopy PROC

mov rdi, rcx
mov rsi, rdx
mov rcx, 8
rep movsq

ret
BCopy ENDP

;---------------------------------------------- BCopy END


;---------------------------------------------- PosDamka

PosDamka PROC

mov rsi, rcx
mov rcx, 4

metka:
	cmp	byte ptr[rsi], 2
	jnz noteq1
	mov byte ptr[rsi], 4
noteq1:
	add rsi, 15
	cmp byte ptr[rsi], 1
	jnz noteq2
	mov byte ptr[rsi], 3
noteq2:
	inc rsi
	loop metka
	
ret
PosDamka ENDP

;---------------------------------------------- PosDamka END

;---------------------------------------------- asmMove

Move PROC
push rbp
mov rbp, rsp

and rdx, 0FFh
and r8, 0FFh
and r9, 0FFh
movsx r10, byte ptr[rbp + 48] 

mov rsi, rcx  
mov rdi, rcx

imul rdx, 8
add rsi, rdx
add rsi, r8

imul r9, 8
add rdi, r9
add rdi, r10

mov al, byte ptr[rsi]
mov byte ptr[rdi], al
mov byte ptr[rsi], 0

call PosDamka

pop rbp
ret
Move ENDP

;---------------------------------------------- asmMove END

;---------------------------------------------- asmBeat

Beat PROC
push rbp
mov rbp, rsp

and rdx, 0FFh
and r8, 0FFh
and r9, 0FFh
movsx r10, byte ptr[rbp + 48] 

mov rsi, rcx  
mov rdi, rcx

imul rdx, 8
add rsi, rdx
add rsi, r8

imul r9, 8
add rdi, r9
add rdi, r10

mov al, byte ptr[rsi]
mov byte ptr[rdi], al
mov byte ptr[rsi], 0

cmp rdx, r9
jg Bigger
jmp Lower

Bigger:
	cmp r8, r10
	jg BBig
	jmp BLow
	BBig:
		sub rsi, 9
		jmp Ending
	BLow:
		sub rsi, 7
		jmp Ending
Lower:
	cmp r8, r10
	jg LBig
	jmp LLow
	LBig:
		add rsi, 7	
		jmp Ending
	LLow:
		add rsi, 9

Ending:
	mov byte ptr [rsi], 0

	call PosDamka


pop rbp
ret
Beat ENDP

;---------------------------------------------- asmBeat END

;---------------------------------------------- SMCheck

SMCheck PROC
push rbp
mov rbp, rsp

xor rax, rax

mov rsi, rcx
mov rcx, [rbp + 48]

call CheckCoord4
test rax, rax
jz Ending

and r9, 0FFh
xor rax, rax

imul r9, 8
add r9b, cl
add rsi, r9

cmp byte ptr[rsi], 0
jnz Ending
mov rax, 1

Ending:

pop rbp
ret
SMCheck ENDP

;---------------------------------------------- SMCheck END

;---------------------------------------------- SBCheck END

SBCheck PROC
push rbp
mov rbp, rsp

mov rsi, rcx                    ;field
mov cl, byte ptr [rbp + 48]     ;y2
mov rdi, rsi

call CheckCoord4
test rax, rax
jz Ending

xor rax, rax

mov r10b, byte ptr [rbp + 56]	;turn
inc r10b
 

and r9, 0FFh
mov r13, r9
imul r13, 8
add r13b, cl
add rdi, r13
mov r12b, byte ptr[rdi]
test r12b, r12b
jnz Ending

;x1 - dl
;y1 - r8b
;x2 - r9b
;y2 - cl
;temp - r11b
;x0 - r12b
;y0 - r13b


mov r12b, dl
mov r13b, r8b 

cmp dl, r9b
jg B
inc r12b

cmp r8b, cl
jg LB
inc r13b
jmp LOR

LB:
dec r13b
jmp LOR

B:
dec r12b
cmp r8b, cl
jg BB
inc r13b
jmp LOR

BB:
dec r13b

LOR:
and r12, 0FFh
imul r12, 8
add r12b, r13b
add rsi, r12

mov r12b, byte ptr[rsi]
cmp r12b, r10b
je Okey
add r10b, 2
cmp r12b, r10b
je Okey
jmp Ending

Okey:
mov rax, 1

Ending:


pop rbp
ret
SBCheck ENDP

;---------------------------------------------- SBCheck END


;---------------------------------------------- getAssess 

getAssess PROC
push rbp
mov rbp, rsp

mov rbx, rcx
mov rsi, OFFSET AssessS
mov rdi, OFFSET AssessD
mov r15, rsi
add r15, 63

xor r8, r8
xor r9, r9
xor r10, r10
xor r11, r11
xor rax, rax

mov rcx, 64
LoopMetka:

mov dl, byte ptr[rbx]
test dl, dl
jz Next

cmp dl, 1
je Piece1

cmp dl, 2
je Piece2

cmp dl, 3
je Piece3

jmp Piece4

Piece1:
add r8b, byte ptr[rsi]
jmp Next

Piece2:
add r9b, byte ptr[r15]
jmp Next

Piece3:
add r10b, byte ptr[rdi]
jmp Next

Piece4:
add r11b, byte ptr[rdi]

Next:
inc rsi
inc rdi
dec r15
inc rbx
loop LoopMetka

xor r12, r12
add r12, r8 
add r12, r10
mov rax, -100
test r12, r12
jz Ending

xor r12, r12
add r12, r9 
add r12, r11
mov rax, 100
test r12, r12
jz Ending

xor r12, r12
mov r12, r10
sub r12, r11
imul r12, 5
add r12, r8
sub r12, r9
mov rax, r12

Ending:

pop rbp
ret
getAssess ENDP

;---------------------------------------------- getAssess END

END


