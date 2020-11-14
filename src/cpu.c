#include"cpu.h"
#include"memory.h"

#include<stdio.h>

extern CPU *cpu;

static int poweroff = 0; // if 1, then poweroff

static void fetch ( void );
static void decode ( void );
static void execute ( void );

// ALU
static uint32_t A,B,*Y;
static void (*operation) ( uint32_t *A, uint32_t *B, uint32_t *Y ) = NULL;

// Arithmetic operations
static void add ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void mlt ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void sub ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void div ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void mod ( uint32_t *A, uint32_t *B, uint32_t *Y );
// Logical operations
static void and ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void or  ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void xor ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void not ( uint32_t *A, uint32_t *B, uint32_t *Y );
// Load & Store
static void ld ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void st ( uint32_t *A, uint32_t *B, uint32_t *Y );
// Jamp
static void jmp ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void jpz ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void jpp ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void jpn ( uint32_t *A, uint32_t *B, uint32_t *Y );
// Other operations
static void hlt ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void nop ( uint32_t *A, uint32_t *B, uint32_t *Y );

static void fetch ( void ) {
  *aBus = cpu->sr.pc++;
  *rw = 0;
  memory_main();
  cpu->sr.ir = *dBus;
  return;
}

static void decode ( void ) {
  uint32_t opcode = ( cpu->sr.ir >> 45 ) & 0x0003FFFF;
  uint32_t immediateFlag = ( cpu->sr.ir >> 44 ) & 0x01;
  uint32_t rx = ( cpu->sr.ir >> 40 ) & 0x0F;
  uint32_t ry = ( cpu->sr.ir >> 36 ) & 0x0F;
  uint32_t rz = ( cpu->sr.ir >> 32 ) & 0x0F;
  uint32_t immediateValue = cpu->sr.ir & 0xFFFFFFFF;
  B = cpu->gr[rx];
  A = cpu->gr[ry];
  Y = &(cpu->gr[rz]);
  if ( immediateFlag ) {
    B = immediateValue;
  }
#ifndef NDEBUG
  int i;
  fprintf(stderr,"[DEBUG]---decode result\n");
  fprintf(stderr,"[DEBUG]opcode=%08X\n",opcode);
  fprintf(stderr,"[DEBUG]immediateFlag=%d\n",immediateFlag);
  fprintf(stderr,"[DEBUG]immediateValue=%08X\n",immediateValue);
  for ( i=0; i<16; i++ ) {
    fprintf(stderr,"[DEBUG]gr[%d]=%08X,@%p\n",i,cpu->gr[i],&(cpu->gr[i]));
  }
  fprintf(stderr,"[DEBUG]rx=%04X\n",rx);
  fprintf(stderr,"[DEBUG]ry=%04X\n",ry);
  fprintf(stderr,"[DEBUG]rz=%04X\n",rz);
  fprintf(stderr,"[DEBUG]A=%08X\n",A);
  fprintf(stderr,"[DEBUG]B=%08X\n",B);
  fprintf(stderr,"[DEBUG]Y=%08X,@%p\n",*Y,Y);
  fprintf(stderr,"[DEBUG]---\n");
#endif
  switch ( opcode ) {
    // Arithmetic operations
    case 0x00008000 : operation = add; break;
    case 0x00008001 : operation = mlt; break;
    case 0x00008002 : operation = sub; break;
    case 0x00008003 : operation = div; break;
    case 0x00008004 : operation = mod; break;
    // Logical operations
    case 0x00010000 : operation = and; break;
    case 0x00010001 : operation = or;  break;
    case 0x00010002 : operation = xor; break;
    case 0x00010003 : operation = not; break;
    // Load & Store
    case 0x00018000 : operation = ld;  break;
    case 0x00018001 : operation = st;  break;
    // Jamp
    case 0x00020000 : operation = jmp; break;
    case 0x00020001 : operation = jpz; break;
    case 0x00020002 : operation = jpp; break;
    case 0x00020003 : operation = jpn; break;
    // other operations
    case 0x00000000 : operation = hlt; break;
    case 0x00000001 : operation = nop; break;
    // other operation, something wrong;
    // so stop machine
    default : operation = hlt; break;
  }
}

static void execute ( void ) {
  printf("IR=%016lX\n",cpu->sr.ir);
  operation(&A,&B,Y);
  printf("A=%016X B=%016X Y=%016X\n",A,B,*Y);
}

// Arithmetic operations
static void add ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = (uint32_t) ( ( (uint64_t)*A + (uint64_t)*B ) & (uint64_t)0xFFFF );
}
static void mlt ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = (uint32_t) ( ( (uint64_t)*A * (uint64_t)*B ) & (uint64_t)0xFFFF );
}
static void sub ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = (uint32_t) ( ( (uint64_t)*A - (uint64_t)*B ) & (uint64_t)0xFFFF );
}
static void div ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = (uint32_t) ( ( (uint64_t)*A / (uint64_t)*B ) & (uint64_t)0xFFFF );
}
static void mod ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = (uint32_t) ( ( (uint64_t)*A % (uint64_t)*B ) & (uint64_t)0xFFFF );
}
// Logical operations
static void and ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = *A & *B;
}
static void or  ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = *A | *B;
}
static void xor ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = *A ^ *B;
}
static void not ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = ~*A;
}
// Load & Store
static void ld ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  // memory[A] -> register_y
  *aBus = *B;
  *rw = 0;
  memory_main();
  *Y = *dBus;
}
static void st ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  // register_A -> memory[Y]
  *dBus = *B;
  *aBus = *Y;
  *rw = 1;
  memory_main();
}
// Jamp
static void jmp ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  cpu->sr.pc = *B;
}
static void jpz ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  if ( *B == 0 ) {
    cpu->sr.pc = *B;
  }
}
static void jpp ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  if ( *B > 0 ) {
    cpu->sr.pc = *B;
  }
}
static void jpn ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  if ( *B < 0 ) {
    cpu->sr.pc = *B;
  }
}
// Other operations
static void hlt ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  cpu->sr.pc--;
  poweroff = 1;
}
static void nop ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
}

void cpu_main ( void ) {
  while ( !poweroff ) {
    fetch();
    decode();
    execute();
  }
}

