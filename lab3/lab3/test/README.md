# Lab 3: Alban FERRACANI

### Overflow times

1. Complete table with overflow times.

$t_{ovf}=\frac{1}{f_{cpu}}\times N \times 2^n$

with $t_{ovf}$ the overflow time, $N$ the prescaler value, $n$ the number of bits.


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

>In the `timer.h`header, I have defined macros for Timer-counter 2. It's a 8 bits timer/counter so, I have calculated the overflows times in function of the prescaler. Here, we can use the following prescalers : `1`, `8`, `32`, `64`, `128`, `256`,`1024` (counters 0 and 1 can't use the `32`and `128`prescalers). 
>Finally, I used the ATmega48A/PA/88A/PA/168A/PA/328/P Data Sheet to know the Clock Select Bit Description (page 165).

   ```c
   /**
    * @name  Definitions for 8-bit Timer/Counter2
    * @note  t_OVF = 1/F_CPU * prescaler * 2^n where n = 8, F_CPU = 16 MHz
    */
   /** @brief Stop timer, prescaler 000 --> STOP */
   #define TIM2_stop()           TCCR2B &= ~((1<<CS22) | (1<<CS21)); TCCR2B |= (1<<CS20);

   /** @brief Set overflow 16us, prescaler 1 -> 001 */ 
   #define TIM2_overflow_16us()    TCCR2B &= ~((1<<CS22) | (1<<CS21)); TCCR2B |= (1<<CS20);

   /** @brief Set overflow 128us, prescaler 8 -> 010 */ 
   #define TIM2_overflow_128us()    TCCR2B &= ~((1<<CS22) | (1<<CS20)); TCCR2B |= (1<<CS21);

   /** @brief Set overflow 512us, prescaler 32 -> 011 */ 
   #define TIM2_overflow_512us()   TCCR2B &= ~(1<<CS22); TCCR2B |= (1<<CS21)|(1<<CS20);

   /** @brief Set overflow 1ms, prescaler 64 -> 100 */ 
   #define TIM2_overflow_1ms()     TCCR2B &= ~((1<<CS21) | (1<<CS20)); TCCR2B |= (1<<CS22);

   /** @brief Set overflow 2ms, prescaler 128 -> 101 */ 
   #define TIM2_overflow_2ms()    TCCR2B &= ~(1<<CS21); TCCR2B |= (1<<CS22)|(1<<CS20);

   /** @brief Set overflow 4ms, prescaler 256 -> 110 */ 
   #define TIM2_overflow_4ms()    TCCR2B &= ~(1<<CS20); TCCR2B |= (1<<CS22)|(1<<CS21);

   /** @brief Set overflow 16ms, prescaler 1024 -> 111 */ 
   #define TIM2_overflow_16ms()   TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20);


   /** @brief Enable overflow interrupt, 1 --> enable */
   #define TIM2_overflow_interrupt_enable()  TIMSK2 |= (1<<TOIE2);
   /** @brief Disable overflow interrupt, 0 --> disable */
   #define TIM2_overflow_interrupt_disable() TIMSK2 &= ~(1<<TOIE2);
   ```
