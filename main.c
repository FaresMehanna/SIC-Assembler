#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

#include "SingleLinkedList.h"
#include "ChainHash.h"
#include "data.h"
#include "operations.h"
#include "prepareDS.h"
#include "helper.h"
#include "phase1.h"
#include "phase2.h"

#include "SingleLinkedList.c"
#include "ChainHash.c"
#include "operations.c"
#include "prepareDS.c"
#include "helper.c"
#include "phase1.c"
#include "phase2.c"

int main(int argc, char* argv[]){

	//the data needed for the functions to work
	assemblerData programData;

	//prepareDataNeeded
	PrepareOperationsHashSet(&(programData.passdata.operations));
	PreparePartsHashSet(&(programData.passdata.parts));
	PreparelabelsHashSet(&(programData.passdata.labels));
	PrepareliteralsHashSet(&(programData.passdata.literals));
	preparelinesLinkedList(&(programData.passdata.lines));
	preparelaterLinkedList(&(programData.passdata.laterLines));
	prepareInstructionsLinkedList(&(programData.passdata.insts));
	initializePassData(&(programData.passdata));

	//print inf to the user
	messagesHandler(newMessage(NOTE_,NOTERMINATE_,MAIN_,strdup("Assembler Started")),&(programData.passdata),1);
	messagesHandler(newMessage(NOTE_,NOTERMINATE_,MAIN_,strdup("DATA INITIALIZED")),&(programData.passdata),1);


	//validate the argv[] data
	validateParameters(argc,argv,&(programData));
	messagesHandler(newMessage(NOTE_,NOTERMINATE_,MAIN_,strdup("PARAMETER VALIDATED")),&(programData.passdata),1);

	//start Pass1
	messagesHandler(newMessage(NOTE_,NOTERMINATE_,MAIN_,strdup("PASS1 IS STARTING")),&(programData.passdata),1);
	Pass1(&programData);

	//start Pass2
	if(!programData.passdata.error)
		messagesHandler(newMessage(NOTE_,NOTERMINATE_,MAIN_,strdup("PASS2 IS STARTING")),&(programData.passdata),1);
	Pass2(&programData);

	//clean up
	messagesHandler(newMessage(NOTE_,NOTERMINATE_,MAIN_,strdup("CLEANUP IS STARTING")),&(programData.passdata),1);
	cleanUp(&(programData.passdata.operations),
			&(programData.passdata.labels),
			&(programData.passdata.parts),
			&(programData.passdata.insts),
			&(programData.passdata.laterLines),
			&(programData.passdata.lines));
}