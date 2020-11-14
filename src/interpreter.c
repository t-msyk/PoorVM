#include<stdio.h>
#include<errno.h>
#include<string.h>
#include"interpreter.h"
#include"memory.h"

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
    fprintf(stderr,"Error string : %s.\n",strerror(error_number));
    fprintf(stderr,"This error occer in FILE=%s,FUNCTION=%s,LINE=%d\n",__FILE__,__func__,__LINE__);
    return 1;
  }
  while ( fgets(buff,255,fp) != NULL ) {
    uint64_t word = 0x00;
    int n;
    int operandCnt=3;
    n = sscanf(buff,"%s %x %x %x",opcode,&x,&y,&z);
#ifndef NDEBUG
    fprintf(stderr,"[DEBUG]%s,%s,%d\n",__FILE__,__func__,__LINE__);
    fprintf(stderr,"[DEBUG]n=%d\n",n);
    fprintf(stderr,"[DEBUG]opcode=%s\n",opcode);
    fprintf(stderr,"[DEBUG]strlen(opcode)=%lu\n",strlen(opcode));
    fprintf(stderr,"[DEBUG]x=%016X\n",x);
    fprintf(stderr,"[DEBUG]y=%016X\n",y);
    fprintf(stderr,"[DEBUG]z=%016X\n",z);
    fflush(stderr);
#endif
    error_number = errno;
    // if error
    if ( n < 1 ) {
      fprintf(stderr,"sscanf failed.(str=%s)\n",buff);
      fprintf(stderr,"Error string : %s.\n",strerror(error_number));
      fprintf(stderr,"This error occer in FILE=%s,FUNCTION=%s,LINE=%d\n",__FILE__,__func__,__LINE__);
      fclose(fp);
      return 1;
    // immidiate values
    } else if ( strncmp(opcode,"0x",2) == 0 ) {
      sscanf(opcode,"%lX",&word);
      memory->cells[i++] = word;
      continue;
    // Arithmetic operations
    } else if ( strcmp(opcode,"ADD" ) == 0 ) { word |= 0x2000000000000000;
    } else if ( strcmp(opcode,"ADDi") == 0 ) { word |= 0x2000020000000000;
    } else if ( strcmp(opcode,"MLT" ) == 0 ) { word |= 0x2000040000000000;
    } else if ( strcmp(opcode,"MLTi") == 0 ) { word |= 0x2000060000000000;
    } else if ( strcmp(opcode,"SUB" ) == 0 ) { word |= 0x2000080000000000;
    } else if ( strcmp(opcode,"SUBi") == 0 ) { word |= 0x20000A0000000000;
    } else if ( strcmp(opcode,"DIV" ) == 0 ) { word |= 0x20000C0000000000;
    } else if ( strcmp(opcode,"DIVi") == 0 ) { word |= 0x20000E0000000000;
    } else if ( strcmp(opcode,"MOD" ) == 0 ) { word |= 0x2000100000000000;
    } else if ( strcmp(opcode,"MODi") == 0 ) { word |= 0x2000120000000000;
    // Logical operations
    } else if ( strcmp(opcode,"AND" ) == 0 ) { word |= 0x4000000000000000;
    } else if ( strcmp(opcode,"ANDi") == 0 ) { word |= 0x4000020000000000;
    } else if ( strcmp(opcode,"OR"  ) == 0 ) { word |= 0x4000040000000000;
    } else if ( strcmp(opcode,"ORi" ) == 0 ) { word |= 0x4000060000000000;
    } else if ( strcmp(opcode,"XOR" ) == 0 ) { word |= 0x4000080000000000;
    } else if ( strcmp(opcode,"XORi") == 0 ) { word |= 0x40000A0000000000;
    } else if ( strcmp(opcode,"NOT" ) == 0 ) { word |= 0x40000C0000000000; operandCnt = 2;
    } else if ( strcmp(opcode,"NOTi") == 0 ) { word |= 0x40000E0000000000; operandCnt = 2;
    // Load & Store
    } else if ( strcmp(opcode,"LD" ) == 0 ) { word |= 0x6000000000000000; operandCnt = 2;
    } else if ( strcmp(opcode,"LDi") == 0 ) { word |= 0x6000020000000000; operandCnt = 2;
    } else if ( strcmp(opcode,"ST" ) == 0 ) { word |= 0x6000040000000000; operandCnt = 2;
    } else if ( strcmp(opcode,"STi") == 0 ) { word |= 0x6000060000000000; operandCnt = 2;
    // Jamp
    } else if ( strcmp(opcode,"JMP" ) == 0 ) { word |= 0x8000000000000000; operandCnt = 2;
    } else if ( strcmp(opcode,"JMPi") == 0 ) { word |= 0x8000020000000000; operandCnt = 2;
    } else if ( strcmp(opcode,"JPZ" ) == 0 ) { word |= 0x8000040000000000;
    } else if ( strcmp(opcode,"JPZi") == 0 ) { word |= 0x8000060000000000;
    } else if ( strcmp(opcode,"JPP" ) == 0 ) { word |= 0x8000080000000000;
    } else if ( strcmp(opcode,"JPPi") == 0 ) { word |= 0x80000A0000000000;
    } else if ( strcmp(opcode,"JPN" ) == 0 ) { word |= 0x80000C0000000000;
    } else if ( strcmp(opcode,"JPNi") == 0 ) { word |= 0x80000E0000000000;
    // other operations
    } else if ( strcmp(opcode,"HLT" ) == 0 ) { word |= 0x0000000000000000; operandCnt = 0;
    } else if ( strcmp(opcode,"HLTi") == 0 ) { word |= 0x0000020000000000; operandCnt = 0;
    } else if ( strcmp(opcode,"NOP" ) == 0 ) { word |= 0x0000040000000000; operandCnt = 0;
    } else if ( strcmp(opcode,"NOPi") == 0 ) { word |= 0x0000060000000000; operandCnt = 0;
    // if other, something wrong
    } else {
      fprintf(stderr,"Invalid opcode %s\n",buff);
      fprintf(stderr,"This error occer in FILE=%s,FUNCTION=%s,LINE=%d\n",__FILE__,__func__,__LINE__);
      fclose(fp);
      return 1;
    }
    if ( n < 1 + operandCnt ) {
      fprintf(stderr,"There are not enough operands.\n");
      fprintf(stderr,"buff=%s, n=%d, operandCnt=%d\n",buff,n,operandCnt);
      fprintf(stderr,"word=%016lX\n",word);
      fprintf(stderr,"This error occer in FILE=%s,FUNCTION=%s,LINE=%d\n",__FILE__,__func__,__LINE__);
      fclose(fp);
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
    word |= ((uint64_t)x&0x07)<<38;
    word |= ((uint64_t)y&0x07)<<35;
    word |= ((uint64_t)z&0x07)<<32;
#ifndef NDEBUG
    fprintf(stderr,"[DEBUG]operandCnt=%d\n",operandCnt);
    fprintf(stderr,"[DEBUG]memory[%d]=%016lX\n",i,word);
#endif
    memory->cells[i++] = word;
  }
  fclose(fp);
  return 0;
}

