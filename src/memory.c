#include"memory.h"

extern Memory *memory;

// TODO implement memory mapped I/O device.
void memory_main ( void ) {
  if ( *rw ) {
    // write
    memory->cells[*aBus] = *dBus;
  } else {
    //read
    *dBus = memory->cells[*aBus];
  }
}



