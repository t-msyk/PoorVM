#include"cpu.h"

/* instruction decode rule
 * top 32 bit and bottom 32 bit
 * bottom 32 bit only use immediate value(or address) specification
 *
 *
 * top 23 bit is instructions
 * bottom 9 bit is register
 *
 * add
 * mult
 * sub
 * dev
 * mod
 *
 * xxx yyy zzz
 *
 * add xxx and yyy, then store result to zzz register.
 *   xxx + yyy -> zzz
 *
 */

void fetch ( void ) {
  MAR = PC++;
  *ABus = MAR;
  *rw = 0;
  memory_main();
  IR = *DBus;
  return;
}

void decode ( void ) {
}

void execute ( void ) {
}

typedef enum InstructoinCycleStage { 
  fetchStage, decodeStage, executeStage
} InstructoinCycleStage;

void cpu_main ( void ) {
  static enum stage
}


