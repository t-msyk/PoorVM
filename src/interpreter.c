#include<stdio.h>
#include<errno.h>
#include<string.h>
#include"interpreter.h"

extern Memory *memory;

int interpreter_main ( char *fname ) {
  FILE *fp;
  int error_number;
  char buff[256];
  char opcode[256];
  uint32_t x,y,z;
  int i=0;
  fp = fopen(fname,"r");
  error_number = errno;
  if ( fp == NULL ) {
    fprintf(stderr,"Could not open %s.\n",fname);
    fprintf(stderr,"Error string : %s.\n"strerror(error_number));
    fprintf(stderr,"This error occer in FILE=%s,FUNCTION=%s,LINE=%d\n",__FILE__,__func__,__LINE__);
    return 1;
  }
  while ( fgets(buff,256,fp) == NULL ) {
    uint64_t word == 0x00;
    int n;
    int operandCnt=3;
    n = sscanf(buff,"%s %x %x %x",opcode,&x,&y,&z);
    error_number = errno;
    if ( n < 1 ) {
      fprintf(stderr,"sscanf failed.(str=%s)\n",buff);
      fprintf(stderr,"Error string : %s.\n"strerror(error_number));
      fprintf(stderr,"This error occer in FILE=%s,FUNCTION=%s,LINE=%d\n",__FILE__,__func__,__LINE__);
      return 1;

    // Arithmetic operations
    } else if ( strcmp(opcode,"ADD") ) {
        word |= 0x2000000000000000;
    } else if ( strcmp(opcode,"ADDi") ) {
        word |= 0x2000020000000000;
    } else if ( strcmp(opcode,"MLT") ) {
        word |= 0x2000040000000000;
    } else if ( strcmp(opcode,"MLTi") ) {
        word |= 0x2000060000000000;
    } else if ( strcmp(opcode,"SUB") ) {
        word |= 0x2000080000000000;
    } else if ( strcmp(opcode,"SUBi") ) {
        word |= 0x20000A0000000000;
    } else if ( strcmp(opcode,"DIV") ) {
        word |= 0x20000C0000000000;
    } else if ( strcmp(opcode,"DIVi") ) {
        word |= 0x20000E0000000000;
    } else if ( strcmp(opcode,"MOD") ) {
        word |= 0x2000100000000000;
    } else if ( strcmp(opcode,"MODi") ) {
        word |= 0x2000120000000000;

    // Logical operations
    } else if ( strcmp(opcode,"AND") ) {
        word |= 0x4000000000000000;
    } else if ( strcmp(opcode,"ANDi") ) {
        word |= 0x4000020000000000;
    } else if ( strcmp(opcode,"OR") ) {
        word |= 0x4000040000000000;
    } else if ( strcmp(opcode,"ORi") ) {
        word |= 0x4000060000000000;
    } else if ( strcmp(opcode,"XOR") ) {
        word |= 0x4000080000000000;
    } else if ( strcmp(opcode,"XORi") ) {
        word |= 0x40000A0000000000;
    } else if ( strcmp(opcode,"NOT") ) {
        word |= 0x40000C0000000000;
        operandCnt = 2;
    } else if ( strcmp(opcode,"NOTi") ) {
        word |= 0x40000E0000000000;
        operandCnt = 2;

    // Load & Store
    } else if ( strcmp(opcode,"LD") ) {
        word |= 0x6000000000000000;
        operandCnt = 2;
    } else if ( strcmp(opcode,"LDi") ) {
        word |= 0x6000020000000000;
        operandCnt = 2;
    } else if ( strcmp(opcode,"ST") ) {
        word |= 0x6000040000000000;
        operandCnt = 2;
    } else if ( strcmp(opcode,"STi") ) {
        word |= 0x6000060000000000;
        operandCnt = 2;

    // Jamp
    } else if ( strcmp(opcode,"JMP") ) {
        word |= 0x8000000000000000;
        operandCnt = 2;
    } else if ( strcmp(opcode,"JMPi") ) {
        word |= 0x8000020000000000;
        operandCnt = 2;
    } else if ( strcmp(opcode,"JPZ") ) {
        word |= 0x8000040000000000;
    } else if ( strcmp(opcode,"JPZi") ) {
        word |= 0x8000060000000000;
    } else if ( strcmp(opcode,"JPP") ) {
        word |= 0x8000080000000000;
    } else if ( strcmp(opcode,"JPPi") ) {
        word |= 0x80000A0000000000;
    } else if ( strcmp(opcode,"JPN") ) {
        word |= 0x80000C0000000000;
    } else if ( strcmp(opcode,"JPNi") ) {
        word |= 0x80000E0000000000;

    // other operations
    } else if ( strcmp(opcode,"HLT") ) {
        word |= 0x0000000000000000;
        operandCnt = 0;
    } else if ( strcmp(opcode,"HLTi") ) {
        word |= 0x0000020000000000;
        operandCnt = 0;
    } else if ( strcmp(opcode,"NOP") ) {
        word |= 0x0000040000000000;
        operandCnt = 0;
    } else if ( strcmp(opcode,"NOPi") ) {
        word |= 0x0000060000000000;
        operandCnt = 0;

    // if other, something wrong
    } else {
      fprintf(stderr,"Invalid opcode %s\n",buff);
      fprintf(stderr,"This error occer in FILE=%s,FUNCTION=%s,LINE=%d\n",__FILE__,__func__,__LINE__);
      return 1;
    }

    if ( n < 1 + operandCnt ) {
      fprintf(stderr,"There are not enough operands.\n",buff);
      fprintf(stderr,"This error occer in FILE=%s,FUNCTION=%s,LINE=%d\n",__FILE__,__func__,__LINE__);
      return 1;
    }
    // if unary operation
    if ( operandCnt == 2 ) {
      z = y;
      y = x;
      x = 7;
    }
    // if immediate value used
    if ( word & 0x0000020000000000 ) {
      word |= y & 0xFFFFFFFF;
      y = 7;
    } 
    word |= (x&0x03)<<38;
    word |= (y&0x03)<<35;
    word |= (z&0x03)<<32;
    memory->cells[i++] = word;
  }
  return 0;
}

