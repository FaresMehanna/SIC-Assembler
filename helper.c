static inline char* strConcat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    assert(result != NULL);

    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

static inline char* charConcat(char *s1, const char s2){
    char *result = malloc(strlen(s1)+2);
    assert(result != NULL);

    strcpy(result, s1);
    result[strlen(s1)] = s2;
    result[strlen(s1)+1] = '\0';

    free(s1);
    return result;
}

static inline char* emptyString(){
	char* empty = malloc(sizeof(char));
	empty[0] = '\0';
	return empty;
}

static inline int strEqual(const char *s1, const char *s2)
{
	if(strlen(s1) != strlen(s2))
		return 0;

	int count = strlen(s1);
	
	for(int i=0;i<count;i++){
		if(s1[i] != s2[i])
			return 0;
	}
	
	return 1;
}

static inline message newMessage(int type,int terminate, int from, char* toWrite){
	
	message x;

	x.type = type;
	x.terminate = terminate;
	x.from = from;
	x.message = toWrite;

	return x;
}

static inline void errorEnd(int num, passData* passdata){
	cleanUp(&(passdata->operations),
		&(passdata->labels),
		&(passdata->parts),
		&(passdata->insts),
		&(passdata->laterLines),
		&(passdata->lines));
	exit(num);
}

static inline void println(){
	printf("\n");
}

static inline void messagesHandler(message x, passData* y, int freeIt){

	char* from;

	if(!HashSetChainSearch(&(y->parts),&(x.from),&from))
		return;

	switch(x.type){
		case 0:
			
			printf(ANSI_COLOR_RED	"ERROR   FROM"	ANSI_COLOR_RESET);
			printf(" %s : %s\n",from,x.message);

			y->error = 1;

			if(x.terminate){
				if(freeIt)
					free(x.message);
				errorEnd(-1,y);
			}
			break;
		case 1:
			printf(ANSI_COLOR_MAGENTA	"WARNING FROM"	ANSI_COLOR_RESET);
			printf(" %s : %s\n",from,x.message);
			break;
		case 2:
			printf(ANSI_COLOR_GREEN		"Note    from"	ANSI_COLOR_RESET);
			printf(" %s : %s\n",from,x.message);
			break;
	}

	if(freeIt)
		free(x.message);
}

static inline int locateString(const char *s1, int len, int* outstart, int* outend){
	int start = 0;
	int end = len-1;
	int exist = 0;

	//get the start point of the label
	for(int i=0;i<len;i++)
		if(s1[i] != ' '){
			start = i;
			exist = 1;
			break;
		}

	//get the end poing of the label
	for(int i=len-1;i!=-1;i--){
		if(s1[i] != ' '){
			end = i;
			exist = 1;
			break;
		}
	}

	*outstart = start;
	*outend = end;
	return exist;
}

static inline char bg(char chr){
	if(chr >= 'a' && chr <= 'z')
		return chr - 'a' + 'A';
	return chr;
}

static inline char sml(char chr){
	if(chr >= 'A' && chr <= 'Z')
		return chr - 'A' + 'a';
	return chr;
}

static inline char* sbg(char* str){
	
	int len = strlen(str);
	for(int i=0;i<len;i++)
		str[i] = bg(str[i]);

	return str;
}

static inline char* ssml(char* str){
	
	int len = strlen(str);
	for(int i=0;i<len;i++)
		str[i] = sml(str[i]);

	return str;
}

static inline int checkPosHex(char* hex){
	
	int len = strlen(hex);

	for(int i=0;i<len;i++){
		if(hex[i] >= '0' && hex[i] <= '9')
			continue;
		if(bg(hex[i]) >= 'A' && bg(hex[i]) <= 'F')
			continue;
		return 0;
	}

	return 1;
}

static inline int checkHex(char* hex){
	
	int len = strlen(hex);

	for(int i=0;i<len;i++){
		if(i == 0)
			if(hex[i] == '+' || hex[i] == '-')
				continue;
		if(hex[i] >= '0' && hex[i] <= '9')
			continue;
		if(bg(hex[i]) >= 'A' && bg(hex[i]) <= 'F')
			continue;
		return 0;
	}

	return 1;
}

static inline int checkSignNum(char* hex){
	
	int len = strlen(hex);

	for(int i=0;i<len;i++){
		if(i == 0)
			if(hex[i] == '+' || hex[i] == '-')
				continue;
		if(hex[i] >= '0' && hex[i] <= '9')
			continue;
		return 0;
	}

	return 1;
}

static inline int checkNum(char* hex){
	
	int len = strlen(hex);

	for(int i=0;i<len;i++){
		if(hex[i] >= '0' && hex[i] <= '9')
			continue;
		return 0;
	}

	return 1;
}

static inline void validateParameters(int argc, char* argv[],assemblerData* x){

	char* in = "-i";
	char* out = "-o";

	if(argc == 3){
		
		if(strEqual(argv[1],in)){
			(x->iodata).inputFile = argv[2];
			(x->iodata).outputFile = "def";
			return;
		}
	}

	else if(argc == 5){

		if(strEqual(argv[1],in)){
			(x->iodata).inputFile = argv[2];
		
			if(strEqual(argv[3],out)){
				(x->iodata).outputFile = argv[4];
				return;
			}
		}
	}

	//report on error
	messagesHandler(newMessage(ERROR_,TERMINATE_,INPUTPARAMETERS_,
	strdup("You can use it in the following ways\n-i inputFileName\n-i inputFileName -o outputFileName\n"))
	,&(x->passdata),1);
}

static inline int offSet10(long x){
	int len = 1;
	int k =1;

	while(x - 10*k >= 0){
		len++;
		k = k * 10;
	}

	return len;
}

static inline int offSet(long x){
	int len = 1;
	int k =1;

	while(x - 16*k >= 0){
		len++;
		k = k * 16;
	}

	return len;
}