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
