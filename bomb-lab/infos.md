# Bomb Lab

%rsp

init_bomb 返回地址

pw1: str @ 0x402400  "Border relations with Canada have never been better."

pw2: 1 2 4 8 16 32

pw3: 0 207

pw4: 7 0 DrEvil

pw5: ionefg

pw6: 4 3 2 1 6 5

x/s 0x603870

DrEvil



# Attack Lab

## 调用顺序

test  --  getbuf -- 注入 -- touch2

​						 |

​						 |

​						  -------- 无注入 -- 返回 test

00000000004017c0 <touch1>:

00000000004017ec <touch2>:

%rdi = cookie

cookie @ 0x6044e4

0x59b997fa

![image-20200729171834955](C:\Users\16529\AppData\Roaming\Typora\typora-user-images\image-20200729171834955.png)

![image-20200729171858131](C:\Users\16529\AppData\Roaming\Typora\typora-user-images\image-20200729171858131.png)

## rtarget

cookie @ 0x6054e4:  0x59b997fa