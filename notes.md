how `one` looks normally:

```
(gdb) disas one
Dump of assembler code for function one:
0x0000000000400b56 <+0>:	    push   %rbp
0x0000000000400b57 <+1>:	    mov    %rsp,%rbp
0x0000000000400b5a <+4>:   mov    $0x2a,%eax
0x0000000000400b5f <+9>:	   pop    %rbp
0x0000000000400b60 <+10>:   retq
```

how `one` looks after I fuck with it:
```
(gdb) disas one
Dump of assembler code for function one:
0x0000000000400b56 <+0>:	    callq  0xfffffffff76086b0
0x0000000000400b5b <+5>:	    sub    (%rax),%al
0x0000000000400b5d <+7>:   add    %al,(%rax)
0x0000000000400b5f <+9>:	   pop    %rbp
0x0000000000400b60 <+10>:   retq

```

at segfault, rip is:
```
rip            0xfffffffff76086b0	0xfffffffff76086b0
```


Can only write 8 bytes atomically. Can't do that while also including an 8 byte address to jump to as well as a ret instruction to patch in. Need to write more atomically. cmpxchg16b? can only write 64 bits with that, which is the same as before

Options:
1. in five bytes, can jump to any 32 bit address. jump to a function which is specially able to then jump to the right place? could do stack unwindiigure out which address it "should" jump to.
1. 

Want to 
