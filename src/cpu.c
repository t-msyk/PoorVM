#include"cpu.h"
#include"memory.h"

extern CPU *cpu;

static poweroff = 0; // if 1, then poweroff

static void fetch ( void );
static void decode ( void );
static void execute ( void );

// ALU
static uint32_t A,B,*Y;
static void (*operation) ( uint32_t *A, uint32_t *B, uint32_t *Y );

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
  uint32_t opcode = ( cpu->sr.ir >> 42 ) & 0x003FFFFF;
  uint32_t immediateFlag = ( cpu->sr.ir >> 41 ) & 0x01;
  uint32_t rx = ( cpu->sr.ir >> 38 ) & 0x07;
  uint32_t ry = ( cpu->sr.ir >> 35 ) & 0x07;
  uint32_t rz = ( cpu->sr.ir >> 32 ) & 0x07;
  uint32_t immediateValue = cpu->sr.ir & 0xFFFFFFFF;
  B = cpu->gr[rx];
  A = cpu->gr[ry];
  Y = &(cpu->gr[rz]);
  if ( immediateFlag ) {
    B = immediateValue;
  }
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
    case 0x00018000 : operation = ld; break;
    case 0x00018001 : operation = st; break;
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
  operation(&A,&B,Y);
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
  *aBus = *A;
  *rw = 0;
  memory_main();
  *Y = *dBus;
}
static void st ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  // register_A -> memory[Y]
  *dBus = *A;
  *aBus = *Y;
  *rw = 1;
  memory_main();
}
// Jamp
static void jmp ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  cpu->sr.pc = *A;
}
static void jpz ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  if ( *B == 0 ) {
    cpu->sr.pc = *A;
  }
}
static void jpp ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  if ( *B > 0 ) {
    cpu->sr.pc = *A;
  }
}
static void jpn ( uint32_t *A, uint32_t *B, uint32_t *Y ) {
  if ( *B < 0 ) {
    cpu->sr.pc = *A;
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

