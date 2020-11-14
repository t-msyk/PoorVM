# PoorVM

Modern computer architecture is too complex to understand for students.
So I create poor and naive architecture virtual machine in this repository.

- Fixed length operation
- All I/O is memory mapped.

## Decode Rule

```
b0 b1 ... b18 b19 b20 ... b23 b24 ... b27 b28 ... b31 b32 b33 ... b63
------------- --- ----------- ----------- ----------- ---------------
^             ^   ^           ^           ^           ^
|             |   |           |           |           |
|             |   |           |           |           +- immediate value.
|             |   |           |           |              If immediate value is not used,
|             |   |           |           |              this field is ignored.
|             |   |           |           |
|             |   |           |           +- register specification.(RZ)
|             |   |           |              Show operation code explain bellow.
|             |   |           |
|             |   |           +- register specification.(RY)
|             |   |              Show operation code explain bellow.
|             |   |
|             |   +- register specification.(RX)
|             |     Show operation code explain bellow.
|             |
|             +- immediate value used flag.
|                If operation use immediate value,
|                this field is 1, else (=not used) this field is 0
|                If this field is 1, use immediate instead of RY;
|                If this field is 1, then add 'i' to mnemonic period.
|                For example, Add -> Addi, LD -> LDi.
|
+- operation code
   operate (RX and) RY, then store result to RZ register.
     Binary Operation:
       for example, operation is subtraction then
         RX - RY -> RZ
     Unary Operation:
       for example, operation is store then
         RY -> memory[RZ]
       operation is load then
         memory[RY] -> RZ
```

   There are 17 operations shown as bellow.
   - Arithmetic operations
     + 0001...0000 : add         (ADD) / Binary Operation
     + 0001...0001 : multiply    (MLT) / Binary Operation
     + 0001...0010 : subtraction (SUB) / Binary Operation
     + 0001...0011 : divivide    (DIV) / Binary Operation
     + 0001...0100 : mod         (MOD) / Binary Operation
   
   - Logical operations
     + 0010...0000 : and         (AND) / Binary Operation
     + 0010...0001 : or          (OR)  / Binary Operation
     + 0010...0010 : xor         (XOR) / Binary Operation
     + 0010...0011 : not         (NOT) / Unary Operation

   - load & store
     + 0011...0000 : load        (LD) / Unary Operation
     + 0011...0001 : store       (ST) / Unary Operation

   - Jamp ( RZ is ignored )  
     Jamp to RY register's value address ( or immediate value address ),
     if RX register is zero/positive/negative.
     + 0100...0000 : jump             (JMP) / Unary Operation
     + 0100...0001 : jump if zero     (JPZ) / Binary Operation
     + 0100...0010 : jump if positive (JPP) / Binary Operation
     + 0100...0011 : jump if negative (JPN) / Binary Operation

   - other operations ( All operand is ignored )
     + 0000...0000 : halt       (HLT) / Binary Operation
     + 0000...0001 : no process (NOP) / Binary Operation

