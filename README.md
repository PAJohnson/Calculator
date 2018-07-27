# Calculator - Patrick Johnson 2018
Simple infix notation text-based calculator without dynamic memory allocation

TODO: Implement a macro expander.

This project is a simple calculator intended for (eventual) use on a microcontroller. As a result, 
it does not use dynamic memory allocation.

Here is the "help" screen:

Supported operations:
+ - * / ^ sin() cos() tan() asin() acos() atan() exp() ln()
For negative numbers: (-10) evaluates to negative 10
Trigonometric operations assume input in radians
type 'quit' to exit program
Example usage: sin(90*(2*3.14159/360)) evaluates to 1.000000

No dependencies besides the C standard library. 
Compile with "gcc -o calc main.c"

How it works:
User input is parsed. The input is evaluated one operator at a time. If only a numeric
value remains, the input is considered evaluated, the answer is printed, and the program
waits for further expressions.

A graphical example of the internal representation leading to an answer
:2+3*4-1
// 3*4 is evaluated first, 12 replaces 3, * and 4 and blanked out
2+12__-1
// the - and 1 are "squished" to the left, eliminating blanks
2+12-1
// 14 replaces 2+12
14__-1
// squish
14-1
// only a numeric value remains, input has been evaluated.
13

While this is not the fastest way to do math on a computer, it requires bounded memory
usage and is suitable for a microcontroller. Eventually, I want to make a physical
calculator as a proof of concept.
