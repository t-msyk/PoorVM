#include"memory.h"
#include<stdlib.h>

extern Memory *memory;

// Bus variables
extern uint64_t *dBus; // data bus
extern uint32_t *aBus; // address bus
extern uint8_t  *rw; // Read(0) or Write(1)

static size_t memory_size=0;

// TODO implement memory mapped I/O device.
void memory_main ( void ) {
  if ( *rw ) {
    // write
    if ( *aBus < memory_size ) {
      memory->cells[*aBus] = *dBus;
    }
  } else {
    //read
    if ( *aBus < memory_size ) {
      *dBus = memory->cells[*aBus];
    }
  }
}

void memory_allocate ( size_t size ) {
  memory_size = size;
  memory = (Memory*) malloc ( sizeof(Memory) + sizeof(uint32_t)*size );
  if ( memory == NULL ) {
    memory_size = 0;
  } else {
    memory_size = size;
  }
}

void memory_free ( void ) {
  memory_size = 0;
  if ( memory ) free(memory);
  memory = NULL;
}

