//will return the wanted function or end the execution
// with error message if the function wasn't found
static inline OberationFunction getFunction(char* opCode,
 HashSetChain* operations,passData* passdata, line* y){
	OberationFunction* x = NULL;
	
	if(!HashSetChainSearch(operations,&opCode,&x)){
		//report on error

		//line num
	    char str2[100];
	    sprintf(str2, "Line -> %d", y->num);	//generate error string

		messagesHandler(newMessage(WARNING_,NOTERMINATE_,PASS1_,(char*)str2),passdata,0);
		messagesHandler(newMessage(WARNING_,NOTERMINATE_,PASS1_,opCode),passdata,0);
		messagesHandler(newMessage(ERROR_,NOTERMINATE_,PASS1_,strdup("Your OpCode NotFound")),passdata,1);
		println();
	}

	return (OberationFunction) x;
}



//will validate the blanks in the instruction
//will end the execution with error message if the blank wasn't there
static inline void ValidateBlank(line* y,passData* passdata){
	if(y->blank != ' ' ||  (y->blank2)[0] != ' '
	||	(y->blank2)[1] != ' '){

		//line num
	    char str2[100];
	    sprintf(str2, "Line -> %d", y->num);	//generate error string

		messagesHandler(newMessage(WARNING_,NOTERMINATE_,PASS1_,(char*)str2),passdata,0);
		messagesHandler(newMessage(WARNING_,NOTERMINATE_,PASS1_,(char*)y),passdata,0);
		messagesHandler(newMessage(ERROR_,NOTERMINATE_,PASS1_,strdup("Error in your Blanks")),passdata,1);
		println();
	}
}

//will get the label of the instruction if found
//will add the label to the hash set with program counter
//will end the execution with error message if the label already defined
static inline void SolveLabel(line* y,
 HashSetChain* labels, int ProgramCounter,passData* passdata){

	int start = 0;
	int end = 7;

	//if there is no label
	if(!locateString(y->label,8,&start,&end))
		return;

	//get the label
	char* label = emptyString();
	for(int i=start ; i != end+1 ; i++)
		label = charConcat(label,(y->label)[i]);
	label = sbg(label);


	//if the label is repeated
	int x = 0;
	if(HashSetChainSearch(labels,&label,&x)){

		//line num
	    char str2[100];
	    sprintf(str2, "Line -> %d", y->num);	//generate error string

		messagesHandler(newMessage(WARNING_,NOTERMINATE_,PASS1_,(char*)str2),passdata,0);
		messagesHandler(newMessage(WARNING_,NOTERMINATE_,PASS1_,(char*)y),passdata,0);
		messagesHandler(newMessage(ERROR_,NOTERMINATE_,PASS1_,strdup("Your label is repeated")),passdata,1);
		println();
		return;
	}

	//add to the hash table with the corrosponding PC
	HashSetChainAddKey(labels,&label,&ProgramCounter);
}

//will get the operation of the instruction
//will add pointer to the function corrosponding to the OpCode if found
//will end the execution with error message if the operation wasn't found
//will end the execution with error message if the operation's Function wasn't found
static inline void SolveOperation(line* y, instruction* x,
 HashSetChain* operations,passData* passdata){

	int start = 0;
	int end = 5;

	//if there is no OpCode
	if(!locateString(y->opCode,6,&start,&end)){

		//line num
	    char str2[100];
	    sprintf(str2, "Line -> %d", y->num);	//generate error string

		messagesHandler(newMessage(WARNING_,NOTERMINATE_,PASS1_,(char*)str2),passdata,0);
		messagesHandler(newMessage(WARNING_,NOTERMINATE_,PASS1_,(char*)y),passdata,0);
		messagesHandler(newMessage(ERROR_,NOTERMINATE_,PASS1_,strdup("No OpCode Found")),passdata,1);
		println();
		x->obFun = empty;
		return;
	}

	//get the opCode
	char* opCode = emptyString();
	for(int i=start ; i != end+1 ; i++)
		opCode = charConcat(opCode,(y->opCode)[i]);

	//set the function to the Instruction
	if(getFunction(sbg(opCode),operations,passdata,y) == NULL)
		x->obFun = empty;
	else
		x->obFun = getFunction(sbg(opCode),operations,passdata,y);

	//free the opCode
	free(opCode);
}

//will get the operands of the instruction if found
//will fix the operand format and add it to the instruction
static inline void SolveOperands(line* y, instruction* x){
	
	int start = 0;
	int end = 17;

	//if there is no Oberands
	if(!locateString(y->operand,18,&start,&end)){
		x->operands = NULL;
		return;
	}

	//get the Oberands
	char* operands = emptyString();

	int spaced = 0;
	for(int i=start ; i != end+1 ; i++){
		if((y->operand)[i] == ' ' || (y->operand)[i] == ',' || (y->operand)[i] == 13){
			//if(!spaced){
				operands = charConcat(operands,' ');
				//spaced = 1;
			//}
		}else{
			//spaced = 0;
			operands = charConcat(operands,(y->operand)[i]);
		}
	}

	//set the operand
	x->operands = sbg(operands);
}
//will call the operationFunction with the given operation number
static inline void Finsh(instruction* x, passData* passdata, int op){
	passdata->ProgramCounter += x->obFun(x,op,NULL,passdata);
}

static inline int checkComment(char* x){
	if(x[0] == '.')
		return 1;
	return 0;
}

//will convert givern String to Instruction struct
//will return pointer to the instruction
//will end the execution with error message if any error with the instruction
static inline instruction* SolveInstruction(char* inst,passData* passdata, int num){

	//check if the given line is a comment
	if(checkComment(inst))
		return NULL;

	//locals
	HashSetChain* operations = &(passdata->operations);
	HashSetChain* labels = &(passdata->labels);
	int * ProgramCounter = &(passdata->ProgramCounter);

	//if the length != 66 report and continue
	if(strlen(inst) != 66){

		//length of the array
	    char str[100];
	    sprintf(str, "length - %d", (int)strlen(inst));	//generate error string

		//line num
	    char str2[100];
	    sprintf(str2, "Line -> %d", num);	//generate error string

	    //send them
		messagesHandler(newMessage(WARNING_,NOTERMINATE_,PASS1_,(char*)str2),passdata,0);
		messagesHandler(newMessage(WARNING_,NOTERMINATE_,PASS1_,(char*)inst),passdata,0);
		messagesHandler(newMessage(WARNING_,NOTERMINATE_,PASS1_,(char*)str),passdata,0);
		messagesHandler(newMessage(ERROR_,NOTERMINATE_,PASS1_,strdup("INSTRUCTION NOT EQUAL 66 Bytes")),passdata,1);
		println();
		return NULL;

	}


	//create Line Struct
	line* y = malloc(sizeof(line));
	//copy data to it
	memcpy(y,inst,strlen(inst)+1);
	y->num = num;

	//create new instruction
	instruction* x = malloc(sizeof(instruction));
	x->lineX = y;	//set the line in the instruction


	ValidateBlank(y,passdata);					//validate blanks
	SolveOperation(y,x,operations,passdata);		//get pointer to the handler function to the opCode
	SolveOperands(y,x);					//get the oberands and fix the format
	Finsh(x,passdata,1);				//call the given function with opNum = 1, //Directives
	SolveLabel(y,labels,*ProgramCounter,passdata);				//add the label with current PC
	x->loc = *ProgramCounter;			//set the location inside the instruction to current PC
	Finsh(x,passdata,2);				//call the given function with opNum = 1, //advance current PC
	
	return x;							//return the generated Instruction
}

//read the data from the file
//create instruction Struct for every Line in the file
void Pass1(assemblerData* programData){

	//local pointers to the Data
	IOData* iodata = &(programData->iodata);
	passData* passdata = &(programData->passdata);
	int* end = &(programData->passdata.end);
	int* start = &(programData->passdata.start);

	//open the file & make sure it's opened
	FILE *file;
	file = fopen(iodata->inputFile,"r");
   	if(file == NULL)
   		messagesHandler(newMessage(ERROR_,TERMINATE_,PASS1_,
   			strdup("Can't open the file to read the data from")),passdata,1)	;

    //read the line char by char
	char cbuff;
	char* sbuff = emptyString();
	do{
		//read char and add it to the string
		cbuff = fgetc(file);
		//if the line ended
		if(cbuff == '\n'){
			//add to the lines LinkedList
			SLinkedListAddBack(&(passdata->lines),&sbuff);
			//create new string for the line
			sbuff = emptyString();
		}else{
			sbuff = charConcat(sbuff,cbuff);
		}

	}while(cbuff != EOF);

	/*
		lines
	*/
	void* IT = SLinkedListGetIterator(&(passdata->lines));
	char* lineTemp;
	int num = 0;
	while(SLinkedListIteratorHasNext(IT)){
		num++;
		SLinkedListIteratorGetNext(IT,&lineTemp);

		*end = 0;
		instruction* x = SolveInstruction(lineTemp,passdata,num);
		*start = *start == 0 ? 2 : *start;
		//add to the linked list if succ
		if(x != NULL)
			//add it to the linked list
			SLinkedListAddBack(&(passdata->insts),&x);
		SLinkedListIteratorGoNext(IT);
	}
	SLinkedListIteratorDispose(IT);

	/*
		later lines
	*/
	IT = SLinkedListGetIterator(&(passdata->laterLines));
	while(SLinkedListIteratorHasNext(IT)){
		num++;
		SLinkedListIteratorGetNext(IT,&lineTemp);
		instruction* x = SolveInstruction(lineTemp,passdata,num);
		*start = *start == 0 ? 2 : *start;
		//add to the linked list if succ
		if(x != NULL)
			//add it to the linked list
			SLinkedListAddBack(&(passdata->insts),&x);
		SLinkedListIteratorGoNext(IT);
	}
	SLinkedListIteratorDispose(IT);

	//fix the last instruction
	if(strlen(sbuff) == 67)
		sbuff[66] = '\0';

	//solve the last instruction
	instruction* x = SolveInstruction(sbuff,passdata,num);
	*start = *start == 0 ? 2 : *start;
	
	//add to the linked list if succ
	if(x != NULL)
		//add it to the linked list
		SLinkedListAddBack(&(passdata->insts),&x);	free(sbuff);

	//if end isn't the last sentence stop execution and yield an error
	if(!*end){
		messagesHandler(newMessage(ERROR_,NOTERMINATE_,PASS1_,
			strdup("END instruction must be the last instruction in your code")),passdata,1);
		println();
	}
	
	//clean up
	fclose(file);
}