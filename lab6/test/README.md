# Lab 6: Alban FERRACANI

### ASCII

1. Complete the table with selected ASCII codes.

   | **Char** | **Decimal** | **Hexadecimal** | **Binary** |
   | :-: | :-: | :-: | :-: |
   | `a` | 97 | 0x61 | `0b0110_0001` |
   | `b` | 98 | 0x62 | `0b0110_0010`|
   | `c` | 99 | 0x63 | `0b0110_0011`|
   | `0` | 48 | 0x30 | `0b0011_0000` |
   | `1` | 49 | 0x31 | `0b0011_0001` |
   | `2` | 50 | 0x32 | `0b0011_0010`|
   | `Esc` | 27 | 0x1b | `0b0001_1011` |
   | `Space` | 32 | 0x20  | `0b0010_0000` |
   | `Tab` | 9 | 0x09 | `0b0000_1001` |
   | `Backspace` | 8 | 0x08 | `0b0000_1000` |
   | `Enter` | 13 | 0x0d | `0b0000_1101` |
   | `Enter` | 10 | 0x0a | `0b0000_1010` |

### UART communication

2. Draw timing diagram of the output from UART/USART when transmitting three character data `De2` in 4800 7O2 mode (7 data bits, odd parity, 2 stop bits, 4800&nbsp;Bd). The image can be drawn on a computer or by hand. Name all parts timing diagram.

>For this question, I transmitt with UART three character data `De2` in 4800 `7O2` mode (7 data bits, odd parity, 2 stop bits). Also, I made the timing diagram on [WaveDrom](https://wavedrom.com/editor.html).
>We transmit three characters `D`,`e`, `2` with a UART 7O2 link.

   | **Char** | **Decimal** | **Hexadecimal** | **Binary** |**Odd parity bit** |
   | :-: | :-: | :-: | :-: | :-: |
   | `D` | 68 | 0x44 | `0b100_0100` |1|
   | `e` | 101 | 0x65 | `0b110_0101`|1|
   | `2` | 50 | 0x32 | `0b011_0010`|0|

>On the real UART link, all the characters `D`,`e`, `2` are sent one after one with 7O2 protocol, so I should represent all the `D`,`e`, `2` chronograms on the same axis (but it would be too long for the screen, that's why I used arrows).
>If we are waiting to transmit data, the signal is in the high state and I have represented it here by the grey hatched signal for better readability.

![wavedrom (1)](https://user-images.githubusercontent.com/114081879/200139182-02bc73e7-94f4-46a2-9389-34184121da9f.svg)


```c
{signal: [
  {name: 'clock', wave: 'p.............'},
  
  {name: 'data char "D"', wave: '.1010..10.1.. ',node: '..............a'},
  {},
  {},
  {name: 'data char "e"', wave: '.101.0.101... ',node: 'b.............c'},
  {},
  {},
  {name: 'data char "2"', wave: '.10.1.0.10.1. ',node: 'd.............e'},
  {},
  { name: "transmission typology",        wave: "x.34444444566x", data: ["start", "D0", "D1", "D2","D3","D4","D5","D6","parity","stop","stop"] },
],
  edge: [
    'a-~>b', 'c-~>d'
  ]
}
```
>So as we transmit the characters 'D', 'e' and '2' one after the other, it gives the following timing diagram (refer to figure 1 to have a bigger image, I decomposed the 'D', 'e', '2' transmission into three lines for more visibility.

![wavedrom (2)](https://user-images.githubusercontent.com/114081879/200139354-72ee1e77-937b-453e-8439-75e41dd123ab.svg)

3. Draw a flowchart for function `uint8_t get_parity(uint8_t data, uint8_t type)` which calculates a parity bit of input 8-bit `data` according to parameter `type`. The image can be drawn on a computer or by hand. Use clear descriptions of individual algorithm steps.

   ![your figure]()
