#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

#include"cpu.h"
#include"memory.h"

// Bus variables
uint64_t *dBus; // data bus
uint32_t *aBus; // address bus
uint8_t  *rw; // Read(0) or Write(1)

Memory *memory;
CPU    *cpu;

int main ( int argc, char *argv[] ) {
  int i;
  memory = (Memory*) malloc ( sizeof(Memory) + sizeof(uint64_t)*1024 );
  cpu    = (CPU*)    malloc ( sizeof(CPU) );
  for ( i=1; i<argc; i++ ) {
    interpreter_main(argv[i]);
    cpu_main();
  }
  return 0;
}
