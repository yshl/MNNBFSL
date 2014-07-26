MNNBFSL
=======

Mada Namae no Nai BrainFuck fu Stack Language

## Stack
MNNBFSL interpreter has 2 stacks, a data stack and a return stack.

## Commands
There are 9 commands and 1 debug command.

|character|meaning|
|---------|-------|
|"|duplicate a value on the top of the data stack|
|+|increment a value on the top of the data stack|
|-|decrement a value on the top of the data stack|
|&gt;|pop a value from the data stack and push the value to the return stack|
|&lt;|pop a value from the return stack and push the value to the data stack|
|[|push a program counter to the return stack|
|]|pop a value from the data stack, if the value is 0, pop a value from the return stack; otherwise, pop a value from the return stack and set the value to the program counter|
|.|pop a value from the data stack and write to stdout|
|,|read 1 byte from stdin, push the byte to the data stack, and push 1 to the data stack. if the end of stdin, push 0 to the data stack|
|s|print the data stack and the return stack to stderr (for debugging)|
|others|no operations|
