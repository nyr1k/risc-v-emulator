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
  sub rsp, 84 ; elf_header (52 bytes) & program_header (32 bytes) 

  ; save arguments before syscalls
  mov r12d, edi
  mov r13, rsi 


; open the ELF file
  mov eax, 2 
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
  mov eax, 0 
  mov edi, ebx 
  lea rsi, [rbp-52] ; buffer 
  mov edx, 52  
  syscall
  ; return -2 if read fail
  cmp eax, 52
  mov edi, -2
  jne .return_error

; check the ELF header
  lea rdi, [rbp-52] ; pass elf_header 
  sub rsp, 12 ; create struct for e_entry, e_phoff, e_phentrysz, e_phnum
  lea rsi, [rsp-96] 
  call check_header
  cmp eax, 0
  mov edi, eax
  jl .return_error
  mov r14, rsi ; save the struct in r14


; traverse program headers   
  mov r8d, 1 
.ph_loop: 
  cmp r8w, word[r14+10] ; number of entries 
  jg .ph_done

; read a program header entry 
  mov eax, 0 
  mov edi, ebx 
  lea rsi, [rbp-84] ; program header buffer
  mov dx, word[r14+8] ; entry_sz  
  syscall 
  ; return -3 if read fail
  cmp eax, 32 
  mov edi, -3
  jne .return_error


; if not PT_LOAD -> read next entry 
  cmp dword[rbp-84], 1 ; PT_LOAD
  jne .ph_continue 


; check p_filesz <= p_memsz, otherwise return error
  mov r9d, dword[rbp-64] ; p_memsz
  cmp dword[rbp-68], r9d
  mov edi, -4
  jg .return_error
  
; check ram_offset <= ram 
  mov r10d, dword[rbp-76] ; p_vaddr 
  sub r10d, r12d ; ram_offset = p_vaddr - base_address
  cmp r10d, ecx 
  mov edi, -5
  jg .return_error

  ; check p_memsz <= ram - ram_offset
  mov r9d, ecx 
  sub r9d, r10d 
  cmp dword[rbp-64], r9d
  mov edi, -5
  jg .return_error 


.load_segment:
; calculate the current offset in the file
  xor r9d, r9d 
  mov r9w, word[r14+8] ; e_phentsize
  imul r9d, r8d 
  add r9d, dword[r14+4] ; add e_phoff
  
; move to the loadable segment via lseek()
  mov eax, 8 
  mov edi, ebx  
  mov esi, dword[rbp-80] ; segment offset in the file image
  mov edx, 0 
  syscall
  ; if moved to incorrect offset -> error
  cmp eax, dword[rbp-80]
  mov edi, -6 
  jne .return_error


; copy the segment into the memory 
  mov eax, 0 
  mov edi, ebx 
  add r10, r13 ; p_offset + *memory 
  mov rsi, r10 
  mov edx, dword[rbp-68] ; read p_filesz bytes
  syscall
  cmp eax, dword[rbp-68]
  mov edi, -7
  jne .return_error


  mov eax, dword[rbp-68] ; p_filesz
  cmp eax, dword[rbp-68] ; p_filesz = p_memsz ?
  je .fill_bss_done
  
  add r10, rax ; bss_start
  mov eax, dword[rbp-64]
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
  mov eax, 8 
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

  mov eax, dword[r14] ; entry_point 

  add rsp, 96
  pop r14
  pop r13
  pop r12
  pop rbx
  pop rbp
  ret 

.return_error:
  add rsp, 96
  pop r14
  pop r13
  pop r12
  pop rbx
  pop rbp
  mov eax, edi
  ret


check_header:
  ; check magic numbers
  cmp dword[rdi], 0x464c457f ; this hex is '0x7f ELF' in little-endian mode
  mov eax, -100 ; not a elf_file
  jne .done 
  
  ; check bitness and endian mode 
  cmp word[rdi+4], 0x0101 ; 32-bit and little-endian 
  mov eax, -99 ; not 32 but and little endian
  jne .done  

  ; file type (must be executalbe) and architecture (must be RISC-V) 
  cmp dword[rdi+16], 0x00F30002 ; 0x02 = Executable file
  mov eax, -98 ; not riscv and not executable 
  jne .done

  ; if everything's correct -> copy the program header fields
  mov eax, [rdi+24]
  mov dword[rsi], eax 
  
  mov eax, [rdi+28]
  mov dword[rsi+4], eax
  
  mov ax, [rdi+42]
  mov word[rsi+8], ax
  
  mov ax, [rdi+44]
  mov word[rsi+10], ax

  mov eax, 0

.done:
  ret
