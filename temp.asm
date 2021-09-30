segment .data
    _x dd 0 ; int x = 0
    _y dd 0 ; int y = 0

segment .bss
    __esp resd 1
    _z resd 1 ; int z

segment .text
	global main
	extern scan_int, print_int, scan_boolean, print_boolean
	extern print_endofline, print_blank, print_string

main:
	mov dword [__esp], esp
    
    ; while(y < 10)
while_inicio: 
    push dword [_y]
    push dword 10
    pop eax
    pop ebx
    cmp ebx, eax
    jge while_fin

    ; y = y + 1
    push dword [_y]
    push dword 1
    pop eax
    pop ebx
    add eax, ebx
    mov [_y], eax

    ; scanf(z)
    push dword _z
    call scan_int
    add esp, 4

    ; if (z > x)
if_inicio:
    push dword [_z]
    push dword [_x]
    pop eax
    pop ebx
    cmp ebx, eax
    jle if_fin

    ; x = z
    push dword [_z]
    pop eax
    mov [_x], eax

if_fin:

    jmp near while_inicio
while_fin:
    
    ; print(x)
	push dword [_x]
	call print_int
	add esp, 4 

programa_fin:
	mov esp, [__esp]
    ret



if eax == 0:
    eax != 0
else
    eax = 0

11111111111
00000000000