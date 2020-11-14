#ifndef __HEADER_MEMORY
#define __HEADER_MEMORY

#include<stdint.h>

extern uint64_t *dBus; // data bus
extern uint32_t *aBus; // address bus
extern uint8_t  *rw; // Read(0) or Write(1)

typedef struct Memory {
  uint64_t cells[0];
} Memory;

void memory_main ( void );

#endif
