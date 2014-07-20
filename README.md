MNNBFSL
=======

Mada Namae no Nai BrainFuck fu Stack Language

# Stack
MNNBFSL interpreter has 2 stacks, stack and rstack.

# Commands
There are 9 commands and 1 debug command.
<dl>
<dt>"</dt><dd>duplicate value on top of the stack</dd>
<dt>+</dt><dd>increment value on top of the stack</dd>
<dt>-</dt><dd>decrement value on top of the stack</dd>
<dt>&gt;</dt><dd>pop from the stack and push to the rstack</dd>
<dt>&lt;</dt><dd>pop from the rstack and push to the stack</dd>
<dt>[</dt><dd>push program counter to the rstack</dd>
<dt>]</dt><dd>pop from the stack, if the value is 0, pop from the rstack; otherwise, pop from the rstack and set to the program counter</dd>
<dt>.</dt><dd>pop from stack and write to stdout</dd>
<dt>,</dt><dd>read from stdin and push to stdin</dd>
<dt>s</dt><dd>print the stack and the rstack to stderr (for debugging)</dd>
<dt>others</dt><dd>no operation</dd>
</dl>
