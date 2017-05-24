/*
 * all the following functions implementing the following form
 * typedef int (*OberationFunction) (void*, int, files*, passData*);
 * each function will hanlde single instruction
 */

#ifndef _OPERATIONS_
#define _OPERATIONS_

#include "data.h"
#include "helper.h"

int LDA (void* instructionX, int operation, files*, passData*);
int STA (void* instructionX, int operation, files*, passData*);
int ADD (void* instructionX, int operation, files*, passData*);
int AND (void* instructionX, int operation, files*, passData*);
int COMP (void* instructionX, int operation, files*, passData*);
int DIV (void* instructionX, int operation, files*, passData*);
int J (void* instructionX, int operation, files*, passData*);
int JEQ (void* instructionX, int operation, files*, passData*);
int JGT (void* instructionX, int operation, files*, passData*);
int JLT (void* instructionX, int operation, files*, passData*);
int JSUB (void* instructionX, int operation, files*, passData*);
int LDCH (void* instructionX, int operation, files*, passData*);
int LDL (void* instructionX, int operation, files*, passData*);
int LDX (void* instructionX, int operation, files*, passData*);
int MUL (void* instructionX, int operation, files*, passData*);
int OR (void* instructionX, int operation, files*, passData*);
int RD (void* instructionX, int operation, files*, passData*);
int RSUB (void* instructionX, int operation, files*, passData*);
int STCH (void* instructionX, int operation, files*, passData*);
int STL (void* instructionX, int operation, files*, passData*);
int STX (void* instructionX, int operation, files*, passData*);
int SUB (void* instructionX, int operation, files*, passData*);
int TD (void* instructionX, int operation, files*, passData*);
int TIX (void* instructionX, int operation, files*, passData*);
int WD (void* instructionX, int operation, files*, passData*);
int START (void* instructionX, int operation, files*, passData*);
int END (void* instructionX, int operation, files*, passData*);
int BYTE (void* instructionX, int operation, files*, passData*);
int WORD (void* instructionX, int operation, files*, passData*);
int RESB (void* instructionX, int operation, files*, passData*);
int RESW (void* instructionX, int operation, files*, passData*);
int empty (void* instructionX, int operation, files*, passData*);
int EQU (void* instructionX, int operation, files*, passData*);
int ORG (void* instructionX, int operation, files*, passData*);
int LTORG (void* instructionX, int operation, files*, passData*);

#endif