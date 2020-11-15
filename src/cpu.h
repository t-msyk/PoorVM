#ifndef __HEADER__CPU
#define __HEADER__CPU

#include<stdint.h>

#pragma pack(push)
#pragma pack(1)
typedef union CPU {
  uint32_t gr[16];       // general (porpose) register
  struct {
    uint32_t pc:32;  // Program Counter         : gr[0]
    uint64_t ir:64; // Instruction Register     : gr[1-2]
    uint32_t reserved[13]; // gr[3-15]
  } sr; // special (porpose) register
} CPU;
#pragma pack(pop)

void cpu_main ( void );
void cpu_allocate ( void );
void cpu_free ( void );

/* 
 * Decode Rule: see README.md
 */

#endif
