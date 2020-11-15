#ifndef __HEADER_MEMORY
#define __HEADER_MEMORY

#include<stdint.h>
#include<stdlib.h>

typedef struct Memory {
  uint64_t cells[0];
} Memory;

void memory_main ( void );
void memory_allocate ( size_t size );
void memory_free ( void );

#endif
