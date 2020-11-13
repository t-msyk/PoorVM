#ifndef __HEADER_MEMORY
#define __HEADER_MEMORY

typedef struct Memory {
  uint64_t *DBus; // data bus
  uint32_t *ABus; // address bus
  uint8_t  *rw; // Read(0) or Write(1)
  uint64_t memory[0];
} Memory;


#endif
