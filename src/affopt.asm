section .text
global asm_crc32
global asm_hash_name 

asm_crc32:
xor eax, eax
test rdx, rdx
jz .done_crc
.loop_crc:
crc32 eax, byte [rcx]
inc rcx
dec rdx
jnz .loop_crc
.done_crc:
ret 

asm_hash_name:
mov eax, 0x811c9dc5
mov r8d, 0x01000193
.loop_hash:
movzx edx, byte [rcx]
test dl, dl
jz .done_hash
xor al, dl
mul r8d
inc rcx
jmp .loop_hash
.done_hash:
ret 

