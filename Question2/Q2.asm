section .data
    filename db "names.txt", 0
    buffer_size equ 4096
    newline db 10
    msg db "Line count: ", 0
    msg_len equ $ - msg
    num_buf db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0     ; for printing int

section .bss
    buffer resb buffer_size
    line_count resd 1

section .text
    global main

main:
    ; your logic here
    ; return from main
    mov rax, 60
    xor rdi, rdi
    syscall

_start:

    ; open("names.txt", O_RDONLY)
    mov rax, 2              ; syscall: sys_open
    mov rdi, filename       ; filename
    mov rsi, 0              ; O_RDONLY
    syscall
    cmp rax, 0
    js _error
    mov r12, rax            ; store file descriptor

    ; read(fd, buffer, buffer_size)
    mov rax, 0              ; syscall: sys_read
    mov rdi, r12            ; fd
    mov rsi, buffer
    mov rdx, buffer_size
    syscall
    cmp rax, 0
    js _error
    mov r13, rax            ; store bytes read

    ; Now parse the buffer
    xor rcx, rcx            ; index in buffer
    xor rbx, rbx            ; line length tracker
    xor eax, eax
    xor r8d, r8d            ; line count

.parse_loop:
    cmp rcx, r13
    jge .done

    mov al, [buffer + rcx]
    inc rcx

    cmp al, 10              ; newline?
    jne .not_newline

    ; end of line found
    inc r8d                 ; increment line count
    xor rbx, rbx            ; reset line char count
    jmp .parse_loop

.not_newline:
    inc rbx
    jmp .parse_loop

.done:
    ; if last char wasn't newline, count last line
    cmp rbx, 0
    je .print

    inc r8d

.print:
    mov [line_count], r8d

    ; print "Line count: "
    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, msg_len
    syscall

    ; convert integer to string
    mov eax, [line_count]
    mov rcx, 10
    lea rsi, [num_buf + 9]
    mov byte [rsi], 10       ; newline
    dec rsi

.convert:
    xor rdx, rdx
    div rcx
    add dl, '0'
    mov [rsi], dl
    dec rsi
    test eax, eax
    jnz .convert

    inc rsi
    ; print number
    mov rax, 1
    mov rdi, 1
    mov rdx, num_buf + 10
    sub rdx, rsi
    syscall

_exit:
    mov rax, 60
    xor rdi, rdi
    syscall

_error:
    ; print error message
    mov rax, 60
    mov rdi, 1
    syscall
