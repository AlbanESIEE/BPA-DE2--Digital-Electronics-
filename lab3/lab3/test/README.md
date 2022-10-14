# Lab 3: Alban FERRACANI

### Overflow times

1. Complete table with overflow times.

>$t_{ovf}=\frac{1}{f_{cpu}}\times N \times 2^n$

>with $t_{ovf}$ the overflow time, $N$ the prescaler value, $n$ the number of bits.

Let's develop the calculus for the fist raw of the table : 

$t_{ovf}=\frac{1}{16\times 10^6}\times 1 \times 2^8 = 1,6\times 10^{-5} = 16uS$

$t_{ovf}=\frac{1}{16\times 10^6}\times 8 \times 2^8 = 1,28\times 10^{-4} = 128uS$

$t_{ovf}=\frac{1}{16\times 10^6}\times 64 \times 2^8 = 1,02\times 10^{-3} = 1mS$

$t_{ovf}=\frac{1}{16\times 10^6}\times 256 \times 2^8 = 4,09\times 10^{-3} = 4mS$

$t_{ovf}=\frac{1}{16\times 10^6}\times 1024 \times 2^8 = 16\times 10^{-3} = 16mS$

   | **Module** | **Number of bits** | **1** | **8** | **32** | **64** | **128** | **256** | **1024** |
   | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
   | Timer/Counter0 | 8  | 16u | 128u | -- | 1m | -- | 4m | 16m |
   | Timer/Counter1 | 16 | 4m | 33m | -- | 262m | -- | 1s | 4.2s |
   | Timer/Counter2 | 8  | 16u | 128u | 512u | 1m | 2m | 4m | 16m |

### Interrupts

2. In `timer.h` header file, define macros also for Timer/Counter2. Listing of part of the header file with settings for Timer/Counter2. Always use syntax highlighting, meaningful comments, and follow C guidelines:

   ```c
   /**
    * @name  Definitions for 8-bit Timer/Counter2
    * @note  t_OVF = 1/F_CPU * prescaler * 2^n where n = 8, F_CPU = 16 MHz
    */
   // WRITE YOUR CODE HERE
   ```
