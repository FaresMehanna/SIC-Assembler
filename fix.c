#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* emptyString(){
	char* empty = malloc(sizeof(char));
	empty[0] = '\0';
	return empty;
}

char* strConcat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);

    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int main(void){
	char elem;
	char* word = emptyString();
	int state = 0;
	while(1){

		scanf("%c",&elem);
		
		if(elem == '9')
			state++;
		else
			state = 0;

		if(state == 4)
			break;
		
		if(elem == '\n'){
			int l = 67-strlen(word);
			for(int i=0;i<strlen(word);i++){
				if( (word[i] >= 65 && word[i] <= 90) ||
				 (word[i] >= 97 && word[i] <= 122) ||
				 (word[i] >= 48 && word[i] <= 57) ||
				 word[i] == 32 ||
				 word[i] == 39 || word[i] == 44
				 || word[i] == '=' || word[i] == '+' || word[i] == '-'
				 || word[i] == '*' || word[i] == '\''){
					printf("%c",word[i]);
				}
				if(word[i] == '\t'){
					l -= 3;
					printf("    ");
				}
			}

			char x = ' ';
			for(int i=0;i<l;i++)
				printf("%c",x);
			printf("\n");
			word = emptyString();
		}
		else{
			char* chr = &elem;
			word = strConcat(word,chr);
			//printf("%s\n",word);
		}


	}
}