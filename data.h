#ifndef _DATADS_
#define _DATADS_

#include "ChainHash.h"
#include "SingleLinkedList.h"

typedef struct orgData
{
	//location of the last index
	int org;
	//flag for the org
	char orgFlag;
}orgData;

typedef struct passData{

	// the programe counter
	int ProgramCounter;

	// flag to know the start of the program
	int start;
	int end;

	//flag to know incase error happend
	int error;

	//for ORG usage
	orgData orgD;

	//HashMap between the operation names and pointer to their handling functions
	HashSetChain operations;
	//HashMap between the labels names and thier memory addresses
	HashSetChain labels;
	//HashMap between the Literals and thier value
	//key is value of the literal and literal name is the data
	HashSetChain literals;
	//HashMap between the parts IDs and their names
	HashSetChain parts;

	//linked list of instruction to be executed in pass2
	SlinkedList insts;

	//linked list of Lines to be executed in pass1 just before the end
	SlinkedList lines;
	//linked list of Lines to be executed in pass1 just before the end
	SlinkedList laterLines;

}passData;


typedef struct IOData{
	//name of the input file
	char* inputFile;
	//name of the output file
	char* outputFile;
}IOData;

typedef struct assemblerData{
	//input and output data
	IOData iodata;
	//the data needed for passes to work
	passData passdata;
}assemblerData;

//types
//0 for error
//1 for warning
//2 for Note
typedef struct message{

	//type of the message
	int type;

	//should the programe terminate immediately ?
	int terminate;

	//which part of the code this message coming from
	int from;

	//the message to display to the user
	char* message;

}message;

//the Instruction line given by the user
typedef struct line{
	char label[8];
	char blank;
	char opCode[6];
	char blank2[2];
	char operand[18];
	char comment[31];
	char end;
	int num;
}line;

//the files to write into
typedef struct files{
	FILE* obj;
	FILE* loc;
}files;

//the operation function which will habdle each OpCode
typedef int (*OberationFunction) (void*, int, files*, passData*);

//the Instruction which will be generated after Pass1
typedef struct instruction{
	char* operands;
	int loc;
	OberationFunction obFun;
	line* lineX;
}instruction;

#endif