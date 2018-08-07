# Calculator - Patrick Johnson 2018
Simple infix notation text-based calculator without dynamic memory allocation

TODO: Implement a macro expander.

This project is a simple calculator intended for (eventual) use on a microcontroller. As a result, 
it does not use dynamic memory allocation.

No dependencies besides the C standard library. 
Compile with "gcc -o calc main.c -lm"

How it works:
User input is parsed. The input is evaluated one operator at a time. If only a numeric
value remains, the input is considered evaluated, the answer is printed, and the program
waits for further expressions.