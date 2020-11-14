#ifndef __HEADER__CPU
#define __HEADER__CPU

#include<stdint.h>

// Bus variables
extern uint64_t *dBus; // data bus
extern uint32_t *aBus; // address bus
extern uint8_t  *rw; // Read(0) or Write(1)

typedef union CPU {
  uint32_t gr[16];       // general (porpose) register
  struct {
    uint32_t pc;  // Program Counter         : gr[0]
    uint64_t ir; // Instruction Register     : gr[1-2]
    uint32_t reserved[13]; // gr[3-15]
  } sr; // special (porpose) register
} CPU;

void cpu_main ( void );

/* 
 * Decode Rule:
 *
 * b0 b1 ... b18 b19 b20 ... b23 b24 ... b27 b28 ... b31 b32 b33 ... b63
 * ------------- --- ----------- ----------- ----------- ---------------
 * ^             ^   ^           ^           ^           ^
 * |             |   |           |           |           |
 * |             |   |           |           |           +- immediate value.
 * |             |   |           |           |              If immediate value is not used,
 * |             |   |           |           |              this field is ignored.
 * |             |   |           |           |
 * |             |   |           |           +- register specification.(RZ)
 * |             |   |           |              Show operation code explain bellow.
 * |             |   |           |
 * |             |   |           +- register specification.(RY)
 * |             |   |              Show operation code explain bellow.
 * |             |   |
 * |             |   +- register specification.(RX)
 * |             |     Show operation code explain bellow.
 * |             |
 * |             +- immediate value used flag.
 * |                If operation use immediate value,
 * |                this field is 1, else (=not used) this field is 0
 * |                If this field is 1, use immediate instead of RY;
 * |                If this field is 1, then add 'i' to mnemonic period.
 * |                For example, Add -> Addi, LD -> LDi.
 * |
 * +- operation code
 *    operate (RX and) RY, then store result to RZ register.
 *      Binary Operation:
 *        for example, operation is subtraction then
 *          RX - RY -> RZ
 *
 *      Unary Operation:
 *        for example, operation is store then
 *          RY -> memory[RZ]
 *        operation is load then
 *          memory[RY] -> RZ
 *
 *    There are 17 operations shown as bellow.
 *    # Arithmetic operations
 *    0001...000 : add         (ADD) / Binary Operation
 *    0001...001 : multiply    (MLT) / Binary Operation
 *    0001...010 : subtraction (SUB) / Binary Operation
 *    0001...011 : divivide    (DIV) / Binary Operation
 *    0001...100 : mod         (MOD) / Binary Operation
 *    
 *    # Logical operations
 *    0010...000 : and         (AND) / Binary Operation
 *    0010...001 : or          (OR)  / Binary Operation
 *    0010...010 : xor         (XOR) / Binary Operation
 *    0010...011 : not         (NOT) / Unary Operation
 *
 *    # load & store
 *    0011...000 : load        (LD) / Unary Operation
 *    0011...001 : store       (ST) / Unary Operation
 *
 *    # Jamp ( RZ is ignored )
 *    0100...000 : jump             (JMP) / Unary Operation
 *    0100...001 : jump if zero     (JPZ) / Binary Operation
 *    0100...010 : jump if positive (JPP) / Binary Operation
 *    0100...011 : jump if negative (JPN) / Binary Operation
 *      Jamp to RY register's value address ( or immediate value address ),
 *      if RX register is zero/positive/negative.
 *
 *    # other operations ( All operand is ignored )
 *    0000...000 : halt       (HLT) / Binary Operation
 *    0000...001 : no process (NOP) / Binary Operation
 *
 */

#endif
