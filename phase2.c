//helper function
//if there is no start this function will be used
static inline void startTemp(files* file, int ProgramCounter){
   fprintf(file->obj,"H^NONAME ^000000^%X",ProgramCounter);
   fprintf(file->loc,"0000\tNONAME   START   0000                                                 ");
}

//it will
//open the files
//handle start statement
//convert each line
void Pass2(assemblerData* programData){

	//local pointers to the Data
	IOData* iodata = &(programData->iodata);
	passData* passdata = &(programData->passdata);

	HashSetChain* operations = &(passdata->operations);
	HashSetChain* labels = &(passdata->labels);
	SlinkedList* insts = &(passdata->insts);

	
	//check for errors from Pass1 return !
	if(passdata->error)
		return;
	
	int* end = &(programData->passdata.end);
	int* start = &(programData->passdata.start);

	void* It = SLinkedListGetIterator(insts);	//iterator to the instructions linkedlist
	files file;		//file struct that will be handled to each operationFunction

	int len = strlen(iodata->outputFile);
	char str[len+5];
	memcpy(str,iodata->outputFile,len);

	//open the first file and validate it
	str[len] = '.';
	str[len+1] = 'o';
	str[len+2] = 'b';
	str[len+3] = 'j';
	str[len+4] = '\0';
	file.obj = fopen(str,"w");
    assert(file.obj != NULL);

	char str2[len+5];
	memcpy(str2,iodata->outputFile,len);
	//open the second file and validate it
	str2[len] = '.';
	str2[len+1] = 'l';
	str2[len+2] = 'o';
	str2[len+3] = 'c';
	str2[len+4] = '\0';
	file.loc = fopen(str2,"w");
    assert(file.loc != NULL);


    int newPc = 0;
    int x = 1;
    int curr = 1;

    //Handle the start 
    //there is no start
    if(*start == 2){
    	startTemp(&file,passdata->ProgramCounter);
    	x = 0;
    	curr = 0;
    }else{
    	if(SLinkedListIteratorHasNext(It)){

			instruction* temp;
			SLinkedListIteratorGetNext(It,&temp);
			SLinkedListIteratorGoNext(It);
			newPc += temp->obFun(temp,4,&file,passdata);
    	
    	}
    }


    //iterate through the linked list with iterator
    //this method will be using two pointers method
    //it will escape RESW,RESB,END in T record
	while(SLinkedListIteratorHasNext(It)){
		
		instruction* temp;

		//if the two pointers met
		//we have to advance one of them and create new T record
		if(x == curr){

			int total = 0;	//total bytes in the record
			int flag = 0;	//have we met RESW,RESB,END ?

			//loop through the linked list
			for(int i=x;i<SLinkedListSize(insts);i++){
				
				//get the next instruction and Execute it with OpNum = 3
				SLinkedListGet(insts,i,&temp);
				int bytes = temp->obFun(temp,3,NULL,passdata);

				//if it's RESW,RESB or something like that
				//make flag = 2
				//stop getting more instruction in this record
				//get more instruction like this to skip it the next time
				//the new record will continue after this one stoped
				if(bytes == -1){
					if(flag == 1){
						flag = 2;
						x = i+1;
						continue;
					}else if(flag == 2){
						x = i+1;
						continue;
					}else if(flag == 0){
						continue;
					}
				}

				//if it's new instruction come after RESW,RESB or something like that
				//break and end the current T record
				if(flag == 2){
					break;
				}

				//if it's a valid instruction check the total size
				//if the size is Ok add it to the current Record
				//else break
				if(total +  bytes <= 30){
					if(flag == 0)
						newPc = temp->loc;
					flag = 1;
					total += temp->obFun(temp,3,NULL,passdata);
					x = i+1;
				}else{
					break;
				}

			}

			fprintf(file.obj, "\nT^");

			//get how many zeros I will need to make it right
			len = offSet(newPc);
			for(int i=len;i<6;i++)
				fprintf(file.obj,"0");
			
			//print the new T record headers
			fprintf(file.obj, "%X^%X^",newPc,total);
			//continue to execute the T record instructions
			continue;
		}

		//if we reach here that mean we are in T record
		//we will get the Node and advance the pointer
		//we will execute the Operation function with OpNum = 4
		//with given Files the function will write to OBJ & LOC files
		curr++;
		SLinkedListIteratorGetNext(It,&temp);
		SLinkedListIteratorGoNext(It);

		newPc += temp->obFun(temp,4,&file,passdata);

	}
	//close the files
	fclose(file.loc);
	fclose(file.obj);

	//if error remove all the files
	if(passdata->error){
		remove(str);
		remove(str2);
	}
	
	//free the pointer to the iterator
	SLinkedListIteratorDispose(It);
}

