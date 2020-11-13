#ifndef __HEADER__CPU
#define __HEADER__CPU

#include<stdint.h>

typedef struct CPU {
  uint64_t IR; // Instruction Register
  uint32_t MAR; // Memory Address Register
  uint32_t PC;  // Program Counter
  GR[8];       // general (porpose) register
  // 
  uint64_t *DBus; // data bus
  uint32_t *ABus; // address bus
  uint8_t  *rw; // Read(0) or Write(1)
} CPU;

void fetch ( void );
void decode ( void );
void execute ( void );
void cpu_main ( void );


#endif
