; Program: Count lines (names) in a file
; File: count_names.asm
; Platform: Linux x86-64
; Assemble: nasm -f elf64 count_names.asm -o count_names.o
; Link: ld count_names.o -o count_names

SECTION .data
    filename db "names.txt", 0           ; file name
    buffer_size equ 4096                 ; size of buffer for reading
    newline db 10                        ; newline character '\n'
    msg db "Number of lines (names): ", 0
    msg_len equ $ - msg

SECTION .bss
    buffer resb buffer_size              ; buffer to hold file content
    line_count resq 1                    ; 8-byte counter

SECTION .text
    global _start

_start:
    ; ----------------------------
    ; 1. Open the file (read-only)
    ; ----------------------------
    mov rax, 2              ; syscall: sys_open
    mov rdi, filename       ; file path
    mov rsi, 0              ; flags = O_RDONLY
    syscall
    cmp rax, 0
    js file_error           ; if rax < 0, jump to error
    mov rdi, rax            ; save file descriptor

    ; ----------------------------
    ; 2. Read file content into buffer
    ; ----------------------------
    mov rax, 0              ; syscall: sys_read
    mov rsi, buffer         ; buffer to store content
    mov rdx, buffer_size    ; number of bytes to read
    syscall
    cmp rax, 0
    jle file_error          ; nothing to read or error
    mov rcx, rax            ; store byte count (for looping)

    ; ----------------------------
    ; 3. Traverse buffer and count lines
    ; ----------------------------
    xor rbx, rbx            ; index = 0
    xor rax, rax            ; line count = 0
.next_char:
    cmp rbx, rcx            ; check if we've reached end of buffer
    jge .done_counting

    mov dl, byte [buffer + rbx]
    cmp dl, 10              ; check if char is '\n'
    jne .continue_loop

    ; Found a newline
    inc rax

.continue_loop:
    inc rbx
    jmp .next_char

.done_counting:
    ; Check for case where file doesn't end with newline but has content
    cmp rcx, 0
    je .skip_final_add
    mov bl, byte [buffer + rcx - 1]
    cmp bl, 10
    je .skip_final_add
    inc rax                ; count last line

.skip_final_add:
    mov [line_count], rax

    ; ----------------------------
    ; 4. Print result
    ; ----------------------------
    ; print message
    mov rax, 1              ; syscall: sys_write
    mov rdi, 1              ; stdout
    mov rsi, msg
    mov rdx, msg_len
    syscall

    ; convert number to string and print
    mov rax, [line_count]
    call print_number

    ; print newline
    mov rax, 1
    mov rdi, 1
    mov rsi, newline
    mov rdx, 1
    syscall

    ; ----------------------------
    ; 5. Exit
    ; ----------------------------
    mov rax, 60             ; syscall: exit
    xor rdi, rdi            ; status 0
    syscall

; ----------------------------
; Error handler
; ----------------------------
file_error:
    mov rax, 60
    mov rdi, 1              ; exit with code 1
    syscall

; ----------------------------
; Helper: Print number in RAX
; ----------------------------
print_number:
    ; Converts RAX to string and prints it
    ; Uses RSI as scratch space
    mov rcx, 10
    lea rsi, [buffer + buffer_size - 1] ; start from end of buffer
    mov byte [rsi], 0

.print_digit:
    xor rdx, rdx
    div rcx
    add dl, '0'
    dec rsi
    mov [rsi], dl
    test rax, rax
    jnz .print_digit

    ; write number string
    mov rax, 1      ; sys_write
    mov rdi, 1      ; stdout
    lea rdx, [buffer + buffer_size - 1]
    sub rdx, rsi    ; string length
    syscall
    ret
