ELF_HEADER equ -54
PH_HEADER equ -84

; CH = check_header()
CH_EI_MAG equ 0
CH_EI_CLASS_DATA equ 4
CH_E_TYPE_MACHINE equ 16
CH_E_ENTRY equ 24
CH_E_PHOFF equ 28
CH_E_PHENTSIZE equ 42
CH_E_PHNUM equ 44

E_ENTRY equ 0
E_PHOFF equ 4 
E_PHENTSIZE equ 8
E_PHNUM equ 10

P_TYPE equ PH_HEADER + 0
P_OFFSET equ PH_HEADER + 4
P_VADDR equ PH_HEADER + 8
P_FILESZ equ PH_HEADER + 16
P_MEMSZ equ PH_HEADER + 20

SYS_READ equ 0
SYS_OPEN equ 2
SYS_CLOSE equ 3
SYS_LSEEK equ 8

section .data 
  testMsg db "PT_LOAD", 10 
section .text
  global load_elf 

; edi - base address (0x80000000), rsi - *memory, rdx *elf_name. ecx - memory size
load_elf:
  push rbp
  push rbx
  push r12
  push r13 
  push r14
  mov rbp, rsp
  sub rsp, 96 ; elf_header (52 bytes) & program_header (32 bytes) & struct elf_info (12 bytes) 

  ; save arguments before syscalls
  mov r12d, edi
  mov r13, rsi 


; open the ELF file
  mov eax, SYS_OPEN
  mov rdi, rdx
  mov esi, 0 
  mov edx, 0 
  syscall
  ; return -1 if open fail
  cmp eax, 0 
  mov edi, -1
  jl .return_error
; save fd 
  mov ebx, eax  


; load the ELF header
  mov eax, SYS_READ 
  mov edi, ebx 
  lea rsi, [rbp + ELF_HEADER]  
  mov edx, 52  
  syscall
  ; return -2 if read fail
  cmp eax, 52
  mov edi, -2
  jne .return_error

; check the ELF header
  lea rdi, [rbp + ELF_HEADER]  
  lea rsi, [rbp-96] 
  call check_header
  cmp eax, 0
  mov edi, eax
  jl .return_error
  mov r14, rsi ; save the struct in r14


; traverse program headers   
  mov r8d, 1 
.ph_loop: 
  cmp r8w, word[r14 + E_PHNUM] 
  jg .ph_done

; read a program header entry 
  mov eax, SYS_READ
  mov edi, ebx 
  lea rsi, [rbp + PH_HEADER] ; program header buffer
  mov dx, word[r14 + E_PHENTSIZE]  
  syscall 
  ; return -3 if read fail
  cmp eax, 32 
  mov edi, -3
  jne .return_error


; if not PT_LOAD -> read next entry 
  cmp dword[rbp + P_TYPE], 1 
  jne .ph_continue 


; check p_vaddr >= base_address
  mov r9d, dword[rbp + P_VADDR]
  cmp r9d, r12d 
  mov edi, -5
  jb .return_error

; check p_filesz <= p_memsz, otherwise return error
  mov r9d, dword[rbp + P_MEMSZ] 
  cmp dword[rbp + P_FILESZ], r9d
  mov edi, -4
  ja .return_error
  
; check ram_offset <= ram 
  mov r10d, dword[rbp + P_VADDR] 
  sub r10d, r12d ; ram_offset = p_vaddr - base_address
  cmp r10d, ecx 
  mov edi, -5
  ja .return_error

  ; check p_memsz <= ram - ram_offset
  mov r9d, ecx 
  sub r9d, r10d 
  cmp dword[rbp + P_MEMSZ], r9d
  mov edi, -5
  ja .return_error 


.load_segment:
; calculate the current offset in the file
  xor r9d, r9d 
  mov r9w, word[r14 + E_PHENTSIZE]
  imul r9d, r8d 
  add r9d, dword[r14 + E_PHOFF]
  
; move to the loadable segment via lseek()
  mov eax, SYS_LSEEK
  mov edi, ebx  
  mov esi, dword[rbp + P_OFFSET] 
  mov edx, 0 
  syscall
  ; if moved to incorrect offset -> error
  cmp eax, dword[rbp + P_OFFSET]
  mov edi, -6 
  jne .return_error


; copy the segment into the memory 
  mov eax, SYS_READ
  mov edi, ebx 
  add r10, r13 ; p_offset + *memory 
  mov rsi, r10 
  mov edx, dword[rbp + P_FILESZ]
  syscall
  cmp eax, dword[rbp + P_FILESZ]
  mov edi, -7
  jne .return_error


  mov eax, dword[rbp + P_FILESZ]
  cmp eax, dword[rbp + P_MEMSZ]
  je .fill_bss_done
  
  add r10, rax ; bss_start
  mov eax, dword[rbp + P_MEMSZ]
  add rax, r10 ; bss_finish

.fill_bss:
  cmp r10, rax 
  je .fill_bss_done

  mov byte[r10], 0 
  
  inc r10
  jmp .fill_bss

.fill_bss_done:

  mov eax, 1
  mov edi, 1
  mov rsi, testMsg
  mov edx, 8
  syscall

; come back to the program header entry 
  mov eax, SYS_LSEEK
  mov edi, ebx 
  mov esi, r9d 
  mov edx, 0 ; SEEK_SET -> calculate from the beginning of the file
  syscall
  ; if moved to incorrect offset -> error 
  cmp eax, r9d
  mov edi, -6
  jne .return_error 


.ph_continue:
  inc r8w
  jmp .ph_loop 

.ph_done: 

  mov eax, SYS_CLOSE
  mov edi, ebx
  syscall 
  cmp eax, 0
  mov edi, -8
  jl .return_error 


  mov eax, dword[r14 + E_ENTRY]

.return:
  add rsp, 96
  pop r14
  pop r13
  pop r12
  pop rbx
  pop rbp
  ret 

.return_error:
  mov eax, edi
  jmp .return


check_header:
  ; check magic numbers
  cmp dword[rdi + CH_EI_MAG], 0x464c457f ; this hex is '0x7f ELF' in little-endian mode
  mov eax, -100 ; not a elf_file
  jne .done 
  
  ; check bitness and endian mode 
  cmp word[rdi + CH_EI_CLASS_DATA], 0x0101 ; 32-bit and little-endian 
  mov eax, -99 ; not 32 but and little endian
  jne .done  

  ; file type (must be executalbe) and architecture (must be RISC-V) 
  cmp dword[rdi + CH_E_TYPE_MACHINE], 0x00F30002 
  mov eax, -98 ; not riscv and not executable 
  jne .done

  ; if everything's correct -> copy the program header fields
  mov eax, [rdi + CH_E_ENTRY]
  mov dword[rsi], eax 
  
  mov eax, [rdi + CH_E_PHOFF]
  mov dword[rsi+4], eax
  
  mov ax, [rdi + CH_E_PHENTSIZE]
  mov word[rsi+8], ax
  
  mov ax, [rdi + CH_E_PHNUM]
  mov word[rsi+10], ax

  mov eax, 0

.done:
  ret
