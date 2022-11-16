# Lab 8: Alban FERRACANI

### Instruction set

1. Complete the conversion table with selected instructions:

    **ADD**
    add r24, r0
    0000 11rd dddd rrr
    d=24 --> 1 1000 in binary
    r=0  --> 0 0000 in binary

    **COM Rd**
    com r26
    1001 010d dddd 0000
    d=26 --> 1 1010 in binary
    
    **EOR Rd,Rr**
    eor r26, r27
    0010	01rd	dddd	rrrr
    d=26 --> 1 1010 in binary
    r=27 --> 1 1011 in binary
   
    **MUL Rd,Rr**
    mul r22, r20
    1001	11rd	dddd  rrrr
    d=22 --> 1 0110 in binary
    r=20 --> 1 0100 in binary
    
    
   | **Instruction** | **Binary opcode** | **Hex opcode** | **Compiler Hex opcode** |
   | :-- | :-: | :-: | :-: |
   | `add r24, r0` | 0000 1101 1000 0000 | 0d80 |  | 
   | `com r26` | 1001 0101 1010 0000 | 95A0 |  |
   | `eor r26, r27` | 0010 0111 1010 1011 | 27AB |  |
   | `mul r22, r20` | 1001 1111 0110 0100 | 9F64 |  |
   | `ret` | `1001_0101_0000_1000` | `95 08` |  |
   

### 4-bit LFSR

2. Complete table with 4-bit LFSR values for different Tap positions:

   | **Tap position** | **Generated values** | **Length** |
   | :-: | :-- | :-: |
   | 4, 3 | `0 1 3 7 14 13 11 6 12 9 2 5 10 4 8`  | Length :15 |
   | 4, 2 | `0 1 3 6 12 8` | Length :6 |
   | 4, 1 | `0 1 2 5 10 4 9 3 6 13 11 7 14 12 8`| Length :15 |

### Variable number of short pulses

3. Draw a flowchart of function `void burst_c(uint8_t number)` which generates a variable number of short pulses at output pin. Let the pulse width be the shortest one. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

   ![your figure]()
