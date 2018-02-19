//compare two strings
static int cmphash (void* P1,void* P2){

	char* f = *(char**)P1;
	char* s = *(char**)P2;

	return strEqual(f,s);
}

//compare two Ints
static int cmpInt (void* P1,void* P2){
	return *(int *)P1 == *(int *)P2;
}
//free the string
static void freeHash(void* f1){
	free(*(char**) f1);
}
//generate hash from string
static unsigned int hash(void* f1, int size, long long max){
	char* f = *(char**)f1;
	unsigned int hash = 2166136261;

	size = strlen(f);

	for(int i=0;i<size;i++){
		hash = hash ^ *(f + i);
 		hash = hash * 16777619;
 	}

	return hash % max;
}
//generate hash from string
static unsigned int hashParts(void* f1, int size, long long max){
	return *(int*)f1 % max;
}
//free instruction
static void FreeInstruction(void* f1){
	instruction* x = *(instruction**)f1;
	free(x->lineX);
	free(x->operands);
	free(x);
}
//prepare the operationshashset
void PrepareOperationsHashSet(HashSetChain* operations){

	InitializeHashSetChain(operations,sizeof(OberationFunction*),sizeof(char*),
		cmphash,NULL,freeHash,hash);

	//temp local variable
	char* temp;
	OberationFunction x;

	//SIC MACHINE
	temp = strdup("LDA");
	x = LDA;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("AND");
	x = AND;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("ADD");
	x = ADD;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("COMP");
	x = COMP;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("DIV");
	x = DIV;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("J");
	x = J;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("JEQ");
	x = JEQ;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("JLT");
	x = JLT;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("JGT");
	x = JGT;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("JSUB");
	x = JSUB;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("LDCH");
	x = LDCH;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("LDL");
	x = LDL;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("LDX");
	x = LDX;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("MUL");
	x = MUL;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("OR");
	x = OR;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("RD");
	x = RD;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("RSUB");
	x = RSUB;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("STA");
	x = STA;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("STCH");
	x = STCH;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("STL");
	x = STL;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("STX");
	x = STX;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("SUB");
	x = SUB;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("TD");
	x = TD;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("TIX");
	x = TIX;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("WD");
	x = WD;
	HashSetChainAddKey(operations,&temp,&x);

	//directives
	temp = strdup("START");
	x = START;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("END");
	x = END;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("BYTE");
	x = BYTE;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("WORD");
	x = WORD;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("RESB");
	x = RESB;
	HashSetChainAddKey(operations,&temp,&x);
	
	temp = strdup("RESW");
	x = RESW;
	HashSetChainAddKey(operations,&temp,&x);

	temp = strdup("ORG");
	x = ORG;
	HashSetChainAddKey(operations,&temp,&x);

	temp = strdup("EQU");
	x = EQU;
	HashSetChainAddKey(operations,&temp,&x);

	temp = strdup("LTORG");
	x = LTORG;
	HashSetChainAddKey(operations,&temp,&x);
}

//prepare the Parts Hashset
void PreparePartsHashSet(HashSetChain* parts){

	InitializeHashSetChain(parts,sizeof(char*),sizeof(int),
		cmpInt,freeHash,NULL,hashParts);

	//temp local variable
	char* temp;
	int x;

	//Pass1
	temp = strdup("PASS 1");
	x = 1;
	HashSetChainAddKey(parts,&x,&temp);

	//Pass2
	temp = strdup("PASS 2");
	x = 2;
	HashSetChainAddKey(parts,&x,&temp);

	//Pass2
	temp = strdup("INPUT PARAMETERS");
	x = 3;
	HashSetChainAddKey(parts,&x,&temp);

	//Pass2
	temp = strdup("MAIN");
	x = 0;
	HashSetChainAddKey(parts,&x,&temp);
	
}

void PreparelabelsHashSet(HashSetChain* labels){
	InitializeHashSetChain(labels,sizeof(int),sizeof(char*),cmphash,NULL,freeHash,hash);
}

void PrepareliteralsHashSet(HashSetChain* literals){
	InitializeHashSetChain(literals,sizeof(int),sizeof(char*),cmphash,NULL,freeHash,hash);
}

void cleanUp(HashSetChain* operations,
 HashSetChain* labels, HashSetChain* Parts, SlinkedList* insts
, SlinkedList* laterLines, SlinkedList* lines){
	DisposeSLinkedList(laterLines);
	DisposeSLinkedList(lines);
	DisposeSLinkedList(insts);
	DisposeHashSetChain(labels);
	DisposeHashSetChain(operations);
	DisposeHashSetChain(Parts);
}

//initialize the LinkedList
void prepareInstructionsLinkedList(SlinkedList* insts){
	InitializeSLinkedList(insts,sizeof(instruction*),FreeInstruction);
}

//initialize the LinkedList
void preparelaterLinkedList(SlinkedList* laterLines){
	InitializeSLinkedList(laterLines,sizeof(char*),freeHash);
}

//initialize the LinkedList
void preparelinesLinkedList(SlinkedList* lines){
	InitializeSLinkedList(lines,sizeof(char*),freeHash);
}


static inline void initializePassData(passData* passdata){
	passdata->start = 0;
	passdata->end = 0;
	passdata->error = 0;
	passdata->ProgramCounter = 0;
	(passdata->orgD).org = 0;
	(passdata->orgD).orgFlag = 0;
}