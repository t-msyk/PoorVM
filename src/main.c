#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

#include"cpu.h"
#include"memory.h"
#include"interpreter.h"

// Bus variables
uint64_t *dBus = NULL; // data bus
uint32_t *aBus = NULL; // address bus
uint8_t  *rw   = NULL; // Read(0) or Write(1)

Memory *memory = NULL;
CPU    *cpu    = NULL;

int allocate_resource ( void ) {
  dBus = (uint64_t*) malloc ( sizeof(uint64_t) );
  aBus = (uint32_t*) malloc ( sizeof(uint32_t) );
  rw   = (uint8_t* ) malloc ( sizeof(uint8_t ) );
  memory_allocate(1024);
  cpu_allocate();
  return 0;
}

int free_resource ( void ) {
  free(dBus);  dBus = NULL;
  free(aBus);  aBus = NULL;
  free(rw  );  rw   = NULL;
  memory_free();
  cpu_free();
  return 0;
}

int main ( int argc, char *argv[] ) {
  int i;
  allocate_resource();
  for ( i=1; i<argc; i++ ) {
    interpreter_main(argv[i]);
    cpu->sr.pc = 0;
    cpu_main();
  }
  free_resource();
  return 0;
}
