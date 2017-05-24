static inline void reportError(instruction* instructionX, passData* passdata,
	char* msg){
	//line num
	char str2[100];
	sprintf(str2, "Line -> %d", instructionX->lineX->num);	//generate error string
    //report errors
	messagesHandler(newMessage(WARNING_,NOTERMINATE_,PASS2_,(char*)str2),passdata,0);
	messagesHandler(newMessage(WARNING_,NOTERMINATE_,PASS2_,(char*)instructionX->lineX),passdata,0);
	messagesHandler(newMessage(ERROR_,NOTERMINATE_,PASS2_,msg),passdata,1);
	println();
}

static inline int getLocationWithX(HashSetChain* labels,
 instruction* instructionX, passData* passdata){
	
	char* operands = instructionX->operands;

	if(operands == NULL)
		return -1;

	int len = strlen(operands);
	char* operand = emptyString();
	int location = -1;

	for(int i=0;i<len;i++){
		if(operands[i] != ' ')
			operand = charConcat(operand,operands[i]);
		else{
			if(location == -1){
				if(!HashSetChainSearch(labels,&operand,&location)){
					//reportError
					reportError(instructionX,passdata,strdup("You operand wasn't defined"));
					//free data
					free(operand);
					operand = emptyString();
					//return
					return -1;
				}
				free(operand);
				operand = emptyString();
			}
			else if(strlen(operand) == 1 && bg(operand[0]) == 'X'){
				int x = 2^16;
				location = location | (32768);
				free(operand);
				operand = emptyString();
			}else{
				//reportError
				reportError(instructionX,passdata,strdup("your operand isn't valid"));
				//free data
				free(operand);
				operand = emptyString();
				//return
				return -1;
			}
		}
	}

	if(location == -1 && !HashSetChainSearch(labels,&operand,&location)){
		//reportError
		reportError(instructionX,passdata,strdup("your operand wasn't defined"));
		//free data
		free(operand);
		operand = emptyString();
		//return
		return -1;
	}

	if(strlen(operand) == 1 && bg(operand[0]) == 'X' && location != -1){
		location  = location | (32768);
	}

	free(operand);
	return location;

}

static inline int getLocationWithoutX(HashSetChain* labels,
 instruction* instructionX, passData* passdata){
	
	char* operands = instructionX->operands;

	if(operands == NULL)
		return -1;

	int location = -1;

	if(!HashSetChainSearch(labels,&operands,&location)){
		//reportError
		reportError(instructionX,passdata,strdup("You operand wasn't defined"));
		//return
		return -1;
	}

	return location;
}

static inline void Normal(HashSetChain* labels
	, void* instructionX, files* file,char* opCode, passData* passdata){
	
	//Location of the instruction & it's operand
	int loc = ((instruction*)instructionX)->loc;
	int OperandLocation = getLocationWithX(labels,(instruction*)instructionX,passdata);

	//if the operand == -1 that mean we have problem founding it
	if(OperandLocation == -1){
		//reportError
		reportError(instructionX,passdata,strdup("this instruction is invalid"));
		return;
	}

	//the line to print in LOC file
	char* lineToPrint = (char *)((instruction*)instructionX)->lineX;
	lineToPrint[strlen(lineToPrint)-1] = '\0';

	//length of the operandLocationg
	int len = offSet(OperandLocation);

	//test for 2 bytes for the oberand
	if(len > 4){
		//reportError
		reportError(instructionX,passdata,strdup("Operand out of range"));
		return;
	}

	//print to the file
	fprintf(file->loc,"\n%X\t%s\t%s",loc,lineToPrint,opCode);
	fprintf(file->obj,"%s",opCode);

	//add the padding zeros
	for(int i=len;i<4;i++){
		fprintf(file->loc,"0");
		fprintf(file->obj,"0");
	}

	//print the values
	fprintf(file->loc,"%X",OperandLocation);
	fprintf(file->obj,"%X^",OperandLocation);
}

static inline void NormalWithNoX(HashSetChain* labels
	, void* instructionX, files* file,char* opCode, passData* passdata){
	
	//Location of the instruction & it's operand
	int loc = ((instruction*)instructionX)->loc;
	int OperandLocation = getLocationWithoutX(labels,(instruction*)instructionX,passdata);

	//if the operand == -1 that mean we have problem founding it
	if(OperandLocation == -1){
		//reportError
		reportError(instructionX,passdata,strdup("this instruction is invalid"));
		return;
	}

	//the line to print in LOC file
	char* lineToPrint = (char *)((instruction*)instructionX)->lineX;
	lineToPrint[strlen(lineToPrint)-1] = '\0';

	//length of the operandLocationg
	int len = offSet(OperandLocation);

	//test for 2 bytes for the oberand
	if(len > 4){
		//reportError
		reportError(instructionX,passdata,strdup("Operand out of range"));
		return;
	}

	//print to the file
	fprintf(file->loc,"\n%X\t%s\t%s",loc,lineToPrint,opCode);
	fprintf(file->obj,"%s",opCode);

	//add the padding zeros
	for(int i=len;i<4;i++){
		fprintf(file->loc,"0");
		fprintf(file->obj,"0");
	}

	//print the values
	fprintf(file->loc,"%X",OperandLocation);
	fprintf(file->obj,"%X^",OperandLocation);
}

static inline void NormalNoOperands(void* instructionX, files* file,char* opCode){
	//location of the instruction
	int loc = ((instruction*)instructionX)->loc;
	//the line to print in loc file
	char* lineToPrint = (char *)((instruction*)instructionX)->lineX;
	lineToPrint[strlen(lineToPrint)-1] = '\0';
	//print to LOC and OBJ files
	fprintf(file->loc,"\n%X\t%s\t%s0000",loc,lineToPrint,opCode);
	fprintf(file->obj,"%s0000^",opCode);
}

static inline void NormalRES(void* instructionX, files* file){
	//location of the instruction
	int loc = ((instruction*)instructionX)->loc;
	//line to print in LOC
	char* lineToPrint = (char *)((instruction*)instructionX)->lineX;
	lineToPrint[strlen(lineToPrint)-1] = '\0';
	//only print it in LOC file
	fprintf(file->loc,"\n%X\t%s\t",loc,lineToPrint);
}

//get label
static inline char* getLabel (line* y){

	int start = 0;
	int end = 7;
	int exist = 0;

	//get the start point of the label
	for(int i=0;i<8;i++)
		if((y->label)[i] != ' '){
			start = i;
			exist = 1;
			break;
		}

	//get the end poing of the label
	for(int i=7;i!=-1;i--){
		if((y->label)[i] != ' '){
			end = i;
			exist = 1;
			break;
		}
	}

	//if there is no label
	if(!exist)
		return NULL;

	//get the label
	char* label = emptyString();
	for(int i=start ; i != end+1 ; i++)
		label = charConcat(label,(y->label)[i]);

	return label;
}

static inline long long SolveCurr(char* curr , int sign, passData* pdata,instruction* instructionX){
	//check if it's the PC
	if(strlen(curr) == 1 && curr[0] == '*')
		return sign * pdata->ProgramCounter;
	//check if it's a number
	else{

		long long location = strtol(curr,NULL,10);
		int loc;

		//it muse be labeld
		if(!checkNum(curr) || LONG_MAX == location || LONG_MIN == location){
			//if it is not a label also then report an error
			if(!HashSetChainSearch(&(pdata->labels),&curr,&loc)){
				//reportError
				reportError(instructionX,pdata
					,strdup("Operand Not Int and Not pre defined label"));
				return LONG_MIN;
			}
			return sign * loc;
		}
		//it is int
		else{
			 return sign * location;
		}
	}
}

static inline long long solveORG(instruction* instructionX, passData* pdata){

		long long total = 0;
		int sign = 3;

		char* line = (char *) instructionX->operands;
		int len = strlen(line);

		char* curr = emptyString();
		
		for(int i=0;i<len;i++){

			switch(line[i]){
				case ' ':

					break;

				case '-':

					//two following -
					if(sign != 3 && strlen(curr) == 0){
						//reportError
						reportError(instructionX,pdata
							,strdup("Error in your expression"));
						return -1;
					}

					if(sign == 3){
						sign = -1;
						break;
					}

					//solve the last label
					long long temp = SolveCurr(curr,sign,pdata,instructionX);

					if(temp == LONG_MIN)
						return 0;
					total += temp;

					free(curr);
					curr = emptyString();

					//set the new sign
					sign = -1;

					break;
				//set the sign = + and check it only come once
				case '+':

					if(sign != 3 && strlen(curr) == 0){
						//reportError
						reportError(instructionX,pdata
							,strdup("Error in your expression"));
						return -1;
					}

					if(sign == 3){
						sign = 1;
						break;
					}

					//solve the last label
					long long temp2 = SolveCurr(curr,sign,pdata,instructionX);
					
					if(temp2 == LONG_MIN)
						return -1;
					total += temp2;

					free(curr);
					curr = emptyString();

					//set the new sign
					sign = 1;

					break;
				//get the string and set the sign to + 
				//if there is no negative or positive found
				default:

					//set to +
					if(sign == 3)
						sign = 1;

					//add the char
					curr = charConcat(curr,line[i]);
					break;
			}
		}

		if(strlen(curr) > 0){
			//solve the last label
			long long temp = SolveCurr(curr,sign,pdata,instructionX);

			if(temp == LONG_MIN)
				return -1;
			total += temp;
		}


		free(curr);
		return total;
}

static inline void SolveLiteral (void* instructionX, passData* pdata){

	//the operand
	char* op = ((instruction*)instructionX)->operands;
	if(op == NULL)
		return;

	//if it's only a NUMBER make it =X'NUMBER'
	if(checkSignNum(op)){
		char* newOp = emptyString();
		//leading
		newOp = charConcat(newOp,'=');
		newOp = charConcat(newOp,'X');
		newOp = charConcat(newOp,'\'');

		int len = strlen(op);
		for(int i=0;i<len;i++){
			newOp = charConcat(newOp,op[i]);
		}

		//ending
		newOp = charConcat(newOp,'\'');

		free(((instruction*)instructionX)->operands);
		((instruction*)instructionX)->operands = newOp;
		op = newOp;
	}

	//it's length
	int len = strlen(op);
	//check for valid format
	if(op[0] != '=' )
		return;
	if(bg(op[1]) != 'X' && bg(op[1]) != 'C' && bg(op[1]) != 'N')
		return;
	if(op[2] != '\'' || op[len-1] != '\'')
		return;

		//get the number
		char* num = emptyString();		
		for(int i=3;i<len-1;i++){
			num = charConcat(num,op[i]);
		}
		int currlen = len-4;

		//check if the data exist
		int temp;
		if(!HashSetChainSearch(&(pdata->literals),&op,&temp)){

			//create the instruction and write it's data
			char* line = malloc(67 * sizeof(char));
			for(int i=0;i<66;i++)
				line[i] = ' ';
			line[66] = '\0';

			int sizeOfHash = HashSetChainSize(&(pdata->literals));
			int newOperand = sizeOfHash+1;
			int offset = offSet10(newOperand);
			int charIndex = 0;
			int d = 1;
			char* newOp = emptyString();

			for(int i=0;i<offset;i++){

				if(i > 0)
					d = d * 10;
				newOp = charConcat(newOp,((newOperand)/d)%10 + '0');
				line[charIndex++] = ((newOperand)/d)%10 + '0'; 
			}

			charIndex = 9;

			if(bg(op[1]) == 'C'){
				line[charIndex++] = 'B';
				line[charIndex++] = 'Y';
				line[charIndex++] = 'T';
				line[charIndex++] = 'E';

				charIndex = 17;
				for(int i=1;i<len;i++)
					line[charIndex++] = op[i];

			}else{
				line[charIndex++] = 'W';
				line[charIndex++] = 'O';
				line[charIndex++] = 'R';
				line[charIndex++] = 'D';

				charIndex = 17;
				for(int i=3;i<len-1;i++)
					line[charIndex++] = op[i];
			}

			//change the current operand
			((instruction*)instructionX)->operands = newOp;

			//add the key as it doesn't found
			HashSetChainAddKey(&(pdata->literals),&op,&newOperand);
			SLinkedListAddBack(&(pdata->laterLines),&line);
		}else{

			int offset = offSet10(temp);
			int d = 1;
			char* newOp = emptyString();

			for(int i=0;i<offset;i++){

				if(i > 0)
					d = d * 10;
				newOp = charConcat(newOp,((temp)/d)%10 + '0');
			}

			//change the current operand
			free(((instruction*)instructionX)->operands);
			((instruction*)instructionX)->operands = newOp;
		}
}

int empty (void* instructionX, int operation, files* file, passData* pdata){

	return 0;	
}

int LDA (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"00", pdata);
	}

	return 3;	
}

int STA (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"0c", pdata);
	}

	return 3;	
}

int ADD (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"18", pdata);
	}

	return 3;	
}

int AND (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"40", pdata);
	}

	return 3;	
}

int COMP (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"28", pdata);
	}

	return 3;	
}

int DIV (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"24", pdata);
	}

	return 3;	
}

int J (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"3c", pdata);
	}

	return 3;	
}

int JEQ (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"30", pdata);
	}

	return 3;	
}


int JGT (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"34", pdata);
	}

	return 3;	
}

int JLT (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"38", pdata);
	}

	return 3;	
}

int JSUB (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"48", pdata);
	}

	return 3;	
}

int LDCH (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"50", pdata);
	}

	return 3;	
}

int LDL (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"08", pdata);
	}

	return 3;	
}

int LDX (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"04", pdata);
	}

	return 3;	
}

int MUL (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"20", pdata);
	}

	return 3;	
}

int OR (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"44", pdata);
	}

	return 3;	
}

int RD (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"d8", pdata);
	}

	return 3;	
}

int RSUB (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){return 0;}

	if(operation == 4){
		NormalNoOperands(instructionX, file,"4c");
	}

	return 3;	
}


int STCH (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"54", pdata);
	}

	return 3;	
}

int STL (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"14", pdata);
	}

	return 3;	
}

int STX (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"10", pdata);
	}

	return 3;	
}

int SUB (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"1c", pdata);
	}

	return 3;	
}

int TD (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"e0", pdata);
	}

	return 3;	
}

int TIX (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"2c", pdata);
	}

	return 3;	
}

int WD (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 1){ SolveLiteral(instructionX,pdata); return 0;}

	if(operation == 4){
		Normal(&(pdata->labels), instructionX, file,"dc", pdata);
	}

	return 3;	
}

int LTORG (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 2){

		/*
			Iterate through lines
		*/
		void* IT = SLinkedListGetIterator(&(pdata->laterLines));
		char* lineTemp;

		for(int i=0;i<SLinkedListSize(&(pdata->laterLines))+i;i++){
			SLinkedListExtractFront(&(pdata->laterLines),&lineTemp);
			SLinkedListInsert(&(pdata->lines),((instruction *)instructionX)->lineX->num,&lineTemp);
		}

	}
	return 0;
}

int START (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 3)
		return -1;

	if(operation == 1){

		//check for dublicate start or Not in the first sentence
		if(pdata->start != 0){
			//reportError
			reportError(instructionX,pdata
				,strdup("START must be used as first instruction only"));
			return 0;
		}

		pdata->start = 1;

		//get the Name to start
		char* label = getLabel(((instruction*)instructionX)->lineX);
		if(label == NULL){
			//reportError
			reportError(instructionX,pdata
				,strdup("NO NAME FOR THE CODE"));
			free(label);
			return 0;
		}
		
		//if there is no Location
		if(((instruction*)instructionX)->operands == NULL)
			return 0;

		//else check the location
		if(!checkHex(((instruction*)instructionX)->operands)){
			//reportError
			reportError(instructionX,pdata
				,strdup("Location in START in wrong"));
			free(label);
			return 0;
		}
		//check the size of the location
		long long location = strtol(((instruction*)instructionX)->operands,NULL,16);
		if(LONG_MAX == location || LONG_MIN == location || location > 16777215){
			//reportError
			reportError(instructionX,pdata
				,strdup("START location is out of range"));
			free(label);
			return 0;
		}

		//free label
		free(label);

		//set the program counter
		return location;
	}

	if(operation == 4){
		
		int loc = ((instruction*)instructionX)->loc;
		
		char temp[7] = "       ";
		temp[6] = '\0';
		
		int len = strlen((char*)((instruction*)instructionX)->lineX);
		memcpy(temp,((instruction*)instructionX)->lineX,len>6?6:len);

		char* lineToPrint = (char *)((instruction*)instructionX)->lineX;
		lineToPrint[strlen(lineToPrint)-1] = '\0';


		fprintf(file->obj,"H^%s^",temp);
		
		len = offSet(loc);
		for(int i=len;i<6;i++)
			fprintf(file->obj,"0");
		fprintf(file->obj,"%X^",loc);

		len = offSet(pdata->ProgramCounter);
		for(int i=len;i<6;i++)
			fprintf(file->obj,"0");
		fprintf(file->obj,"%X\n",pdata->ProgramCounter);


	    fprintf(file->loc,"%X\t%s",loc,lineToPrint);

	    return strtol(((instruction*)instructionX)->operands,NULL,16);
	}

	return 0;
}


int END (void* instructionX, int operation, files* file, passData* pdata){

	//set end flag to one 
	pdata->end = 1;

	//if the operation = 3 then return -1 to not write it in the obj file
	if(operation == 3)
		return -1;

	//write it only in the loc file
	if(operation == 4){
		char* label = ((instruction*)instructionX)->operands;

		int x = 0;
		if(!HashSetChainSearch(&(pdata->labels),&label,&x)){
			//reportError
			reportError(instructionX,pdata
				,strdup("YOUR OPCODE NOT FOUND IN END STATEMENT"));
			return 0;
		}

		int loc = ((instruction*)instructionX)->loc;
		char* lineToPrint = (char *)((instruction*)instructionX)->lineX;
		lineToPrint[strlen(lineToPrint)-1] = '\0';
		fprintf(file->loc,"\n%X\t%s\t",loc,lineToPrint);


		fprintf(file->obj,"\nE^");
		int len = offSet(x);

		for(int i=len;i<6;i++)
			fprintf(file->obj,"0");

		fprintf(file->obj,"%X",x);
	}
	return 0;
}

int BYTE (void* instructionX, int operation, files* file, passData* pdata){

	if(operation != 1){

		//if there is no value
		if(((instruction*)instructionX)->operands == NULL){
			//reportError
			reportError(instructionX,pdata
				,strdup("No Initial Value for BYTE"));
			return 0;
		}

		//check length
		if(strlen(((instruction*)instructionX)->operands) < 3){
			//reportError
			reportError(instructionX,pdata
				,strdup("Error in your Instruction"));
			return 0;
		}

		int len = strlen(((instruction*)instructionX)->operands);

		//if the literal in chars
		if(bg((((instruction*)instructionX)->operands)[0]) == 'C'){

			//check for the begining and the end
			if((((instruction*)instructionX)->operands)[1] != '\'' ||
				(((instruction*)instructionX)->operands)[len-1] != '\''){
				//reportError
				reportError(instructionX,pdata
					,strdup("Error in your Instruction"));
				return 0;
			}

			//write to the file
			if(operation == 4){
				//location of instruction
				int loc = ((instruction*)instructionX)->loc;
				//line to print
				char* lineToPrint = (char *)((instruction*)instructionX)->lineX;
				lineToPrint[strlen(lineToPrint)-1] = '\0';
				//print the headers
				fprintf(file->loc,"\n%X\t%s\t",loc,lineToPrint);
				fprintf(file->obj, "^");
				//print the data
				for(int i=2;i<len-1;i++){
					fprintf(file->loc, "%x", (((instruction*)instructionX)->operands)[i]);
					fprintf(file->obj, "%x", (((instruction*)instructionX)->operands)[i]);
				}
			}
		//if it's hex constant
		}else if(bg((((instruction*)instructionX)->operands)[0]) == 'X'){

			//check for start and end
			if((((instruction*)instructionX)->operands)[1] != '\'' ||
				(((instruction*)instructionX)->operands)[len-1] != '\''){
				//reportError
				reportError(instructionX,pdata
					,strdup("Error in your Instruction"));
				return 0;
			}

			//get the hex data
			char* hex = emptyString();
			for(int i=2;i<len-1;i++){
				hex = charConcat(hex,(((instruction*)instructionX)->operands)[i]);
			}

			//else check the hex for correctness
			if(!checkHex(hex)){
				//reportError
				reportError(instructionX,pdata
					,strdup("Value in BYTE is wrong"));
				return 0;
			}

			//check the size of the value
			long long location = strtol(hex,NULL,16);
			if(LONG_MAX == location || LONG_MIN == location || location > 256 || location < 0){
				//reportError
				reportError(instructionX,pdata
					,strdup("Value in BYTE is out of range"));
				return 0;
			}

			//get the length of bytes
			int lentemp = len;
			len = (len-3)/2 + 3;

			if(operation == 4){
				//location of instruction
				int loc = ((instruction*)instructionX)->loc;
				//line to print
				char* lineToPrint = (char *)((instruction*)instructionX)->lineX;
				lineToPrint[strlen(lineToPrint)-1] = '\0';
				//print headers
				fprintf(file->loc,"\n%X\t%s\t",loc,lineToPrint);
				//print padding zeros
				for(int i=lentemp-3;i<6;i++){
					fprintf(file->loc, "0");
				}
				//print the data
				for(int i=2;i<lentemp-1;i++){
					fprintf(file->loc, "%c", (((instruction*)instructionX)->operands)[i]);
					fprintf(file->obj, "%c", (((instruction*)instructionX)->operands)[i]);
				}
				fprintf(file->obj, "^");
			}
			//free string
			free(hex);
		//unkonwn start will yiels an error
		}else{
			//reportError
			reportError(instructionX,pdata
				,strdup("Error in your Instruction"));
			return 0;
		}

		//return length of the constant
		return len-3;
	}

	return 0;
}

int WORD (void* instructionX, int operation, files* file, passData* pdata){

	if(operation != 1){
		
		//if there is no value
		if(((instruction*)instructionX)->operands == NULL){
			//reportError
			reportError(instructionX,pdata
				,strdup("NO INITIAL VALUE FOR WORD"));
			return 0;
		}

		//else check the value
		if(!checkHex(((instruction*)instructionX)->operands)){
			//reportError
			reportError(instructionX,pdata
				,strdup("value in WORD is wrong"));
			return 0;
		}

		//check the size of the value
		long long location = strtol(((instruction*)instructionX)->operands,NULL,10);
		if(LONG_MAX == location || LONG_MIN == location || location > 8388607 || location < -8388608){
			//reportError
			reportError(instructionX,pdata
				,strdup("value in WORD out of range"));
			return 0;
		}

			if(operation == 4){
				int loc = ((instruction*)instructionX)->loc;
				char* lineToPrint = (char *)((instruction*)instructionX)->lineX;
				lineToPrint[strlen(lineToPrint)-1] = '\0';
				fprintf(file->loc,"\n%X\t%s\t",loc,lineToPrint);

				char temp[100];
				sprintf(temp,"%016llX",location);
				//write to the files
				fprintf(file->loc, "%c%c%c%c%c%c",
				 temp[10], temp[11], temp[12],temp[13],temp[14],temp[15]);
				fprintf(file->obj, "%c%c%c%c%c%c^",
				 temp[10], temp[11], temp[12],temp[13],temp[14],temp[15]);
			}

		return 3;
	}

	return 0;
}

int RESB (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 3)
		return -1;

	if(operation != 1){
		
		//if there is no value
		if(((instruction*)instructionX)->operands == NULL){
			//reportError
			reportError(instructionX,pdata
				,strdup("NO INITIAL VALUE FOR RESB"));
			return 0;
		}

		//else check the value
		if(!checkNum(((instruction*)instructionX)->operands)){
			//reportError
			reportError(instructionX,pdata
				,strdup("value in RESB is wrong"));
			return 0;
		}

		//check the size of the value
		long long location = strtol(((instruction*)instructionX)->operands,NULL,10);
		if(LONG_MAX == location || LONG_MIN == location || location > 999999){
			//reportError
			reportError(instructionX,pdata
				,strdup("value in RESB out of range"));
			return 0;
		}

		if(operation == 4){
			NormalRES(instructionX, file);
		}

		return location;
	}

	return 0;
}

int RESW (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 3)
		return -1;

	if(operation != 1){
		
		//if there is no value
		if(((instruction*)instructionX)->operands == NULL){
			//reportError
			reportError(instructionX,pdata
				,strdup("NO INITIAL VALUE FOR RESW"));
			return 0;
		}

		//else check the value
		if(!checkNum(((instruction*)instructionX)->operands)){
			//reportError
			reportError(instructionX,pdata
				,strdup("value in RESW is wrong"));
			return 0;
		}

		//check the size of the value
		long long location = strtol(((instruction*)instructionX)->operands,NULL,10);
		if(LONG_MAX == location || LONG_MIN == location || location > 999999){
			//reportError
			reportError(instructionX,pdata
				,strdup("value in RESW out of range"));
			return 0;
		}

		if(operation == 4){
			NormalRES(instructionX, file);
		}

		return location*3;
	}

	return 0;
}

//will set the programCounter to the value given in the operands
int ORG (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 3)
		return -1;

	//write to loc file
	if(operation == 4){
		//location of the instruction
		int loc = ((instruction*)instructionX)->loc;
		//the line to print in loc file
		char* lineToPrint = (char *)((instruction*)instructionX)->lineX;
		lineToPrint[strlen(lineToPrint)-1] = '\0';
		//print to LOC and OBJ files
		fprintf(file->loc,"\n\t\t%s",lineToPrint);
	}


	//if it's operation 2 then fix the PC
	if(operation == 2){

		//if there is no operands
		if(((instruction*)instructionX)->operands == NULL || strlen(((instruction*)instructionX)->operands) == 0){
			
			//if it has been used
			if((pdata->orgD).orgFlag){
				//set the programe counter
				pdata->ProgramCounter = (pdata->orgD).org;
				//set the flag
				(pdata->orgD).orgFlag = 0;
				//return
				return 0;
			}

			//reportError
			reportError(instructionX,pdata
				,strdup("No Operand to the ORG statement"));
			return 0;
		}

		//check and set the new data
		int new = solveORG(instructionX,pdata);

		if(new >= 0){
			//set the flag & data
			if(!(pdata->orgD).orgFlag){
				//set backup org data
				(pdata->orgD).org = pdata->ProgramCounter;
				//set the flag to one as there data exists
				(pdata->orgD).orgFlag = 1;
			}

			//set the programe counter
			pdata->ProgramCounter = new;
		}else
			//reportError
			reportError(instructionX,pdata
				,strdup("FAILED TO COMMIT YOUR ORG STATEMENT"));
	}

	return 0;
}


//wil give to the value in the label the value in the operands
int EQU (void* instructionX, int operation, files* file, passData* pdata){

	if(operation == 3)
		return -1;

	if(operation == 4){
		//location of the instruction
		int loc = ((instruction*)instructionX)->loc;
		//the line to print in loc file
		char* lineToPrint = (char *)((instruction*)instructionX)->lineX;
		lineToPrint[strlen(lineToPrint)-1] = '\0';
		//print to LOC and OBJ files
		fprintf(file->loc,"\n\t\t%s",lineToPrint);
	}


	//if it's operation 2 then solve the label for the right location
	if(operation == 2){

		char* label = getLabel(((instruction *)instructionX)->lineX);		//going to use malloc to return the string
		char* operand = ((instruction *)instructionX)->operands;
		int operandLocation = -1;

		//if there is no Label
		if(strlen(label) == 0){
			//reportError
			reportError(instructionX,pdata
				,strdup("No Label to the EQU Statement"));
			free(label);
			return 0;
		}
		
		//if there is no operands
		if(operand == NULL || strlen(operand) == 0){
			//reportError
			reportError(instructionX,pdata
				,strdup("No Operand to the EQU statement"));
			free(label);
			return 0;
		}

		//check and set the new data
		int new = solveORG(instructionX,pdata);

		if(new < 0){
			//reportError
			reportError(instructionX,pdata
				,strdup("FAILED TO COMMIT YOUR EQU STATEMENT"));
		}


		//add the key to the hashSet
		//the label will be updated as it taked the PC in OPERATION 1
		HashSetChainAddKey(&(pdata->labels),&label,&new);
	}

	return 0;
}