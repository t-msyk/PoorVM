#include"cpu.h"
#include"memory.h"

#include<stdio.h>
#include<stdlib.h>

extern CPU *cpu;

// Bus variables
extern uint64_t *dBus; // data bus
extern uint32_t *aBus; // address bus
extern uint8_t  *rw; // Read(0) or Write(1)


static int poweroff = 0; // if 1, then poweroff

static void fetch ( void );
static void decode ( void );
static void execute ( void );

// ALU
static uint32_t A,B,*Y;
static void (*operation) ( uint32_t *A, uint32_t *B, uint32_t *Y ) = NULL;

// Arithmetic operations
static void ADD ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void MLT ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void SUB ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void DIV ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void MOD ( uint32_t *A, uint32_t *B, uint32_t *Y );
// Logical operations
static void AND ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void OR  ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void XOR ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void NOT ( uint32_t *A, uint32_t *B, uint32_t *Y );
// Load & Store
static void LD ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void ST ( uint32_t *A, uint32_t *B, uint32_t *Y );
// Jamp
static void JMP ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void JPZ ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void JPP ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void JPN ( uint32_t *A, uint32_t *B, uint32_t *Y );
// Other operations
static void HLT ( uint32_t *A, uint32_t *B, uint32_t *Y );
static void NOP ( uint32_t *A, uint32_t *B, uint32_t *Y );

static void fetch ( void ) {
  *aBus = cpu->sr.pc++;
  *rw = 0;
  memory_main();
  cpu->sr.ir = *dBus;
  return;
}

static void decode ( void ) {
  uint32_t opcode = ( cpu->sr.ir >> 48 ) & 0x0000FFFF;
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
    case 0x00001000 : operation = ADD; break;
    case 0x00001001 : operation = MLT; break;
    case 0x00001002 : operation = SUB; break;
    case 0x00001003 : operation = DIV; break;
    case 0x00001004 : operation = MOD; break;
    // Logical operations
    case 0x00002000 : operation = AND; break;
    case 0x00002001 : operation = OR;  break;
    case 0x00002002 : operation = XOR; break;
    case 0x00002003 : operation = NOT; break;
    // Load & Store
    case 0x00003000 : operation = LD;  break;
    case 0x00003001 : operation = ST;  break;
    // Jamp
    case 0x00004000 : operation = JMP; break;
    case 0x00004001 : operation = JPZ; break;
    case 0x00004002 : operation = JPP; break;
    case 0x00004003 : operation = JPN; break;
    // other operations
    case 0x00000000 : operation = HLT; break;
    case 0x00000001 : operation = NOP; break;
    // other operation, something wrong;
    // so stop machine
    default : operation = HLT; break;
  }
}

static void print_register ( void ) {
  int i;
  printf("IR=%016lX\n",cpu->sr.ir);
  printf("PC=%016X\n",cpu->sr.pc);
  for ( i=0 ;i<16; i++ ) {
    printf("GR[%d]=%016X\n",i,cpu->gr[i]);
  }
}

static void execute ( void ) {
  getchar();
  print_register();
  operation(&A,&B,Y);
  printf("A=%016X B=%016X Y=%016X\n",A,B,*Y);
}

// Arithmetic operations
static void ADD ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = (uint32_t) ( ( (uint64_t)*A + (uint64_t)*B ) & (uint64_t)0xFFFF );
}
static void MLT ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = (uint32_t) ( ( (uint64_t)*A * (uint64_t)*B ) & (uint64_t)0xFFFF );
}
static void SUB ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = (uint32_t) ( ( (uint64_t)*A - (uint64_t)*B ) & (uint64_t)0xFFFF );
}
static void DIV ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = (uint32_t) ( ( (uint64_t)*A / (uint64_t)*B ) & (uint64_t)0xFFFF );
}
static void MOD ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = (uint32_t) ( ( (uint64_t)*A % (uint64_t)*B ) & (uint64_t)0xFFFF );
}
// Logical operations
static void AND ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = *A & *B;
}
static void OR  ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = *A | *B;
}
static void XOR ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = *A ^ *B;
}
static void NOT ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  *Y = ~*A;
}
// Load & Store
static void LD ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  // memory[A] -> register_y
  *aBus = *B;
  *rw = 0;
  memory_main();
  *Y = *dBus;
}
static void ST ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  // register_A -> memory[Y]
  *dBus = *B;
  *aBus = *Y;
  *rw = 1;
  memory_main();
}
// Jamp
static void JMP ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  cpu->sr.pc = *B;
}
static void JPZ ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  if ( *B == 0 ) {
    cpu->sr.pc = *B;
  }
}
static void JPP ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  if ( *B > 0 ) {
    cpu->sr.pc = *B;
  }
}
static void JPN ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  if ( *B < 0 ) {
    cpu->sr.pc = *B;
  }
}
// Other operations
static void HLT ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  cpu->sr.pc--;
  poweroff = 1;
}
static void NOP ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
}

void cpu_main ( void ) {
  while ( !poweroff ) {
    fetch();
    decode();
    execute();
  }
}

void cpu_allocate ( void ) {
  cpu = (CPU*) malloc ( sizeof(CPU) );
}
void cpu_free ( void ) {
  if ( cpu ) free(cpu);
  cpu = NULL;
}

