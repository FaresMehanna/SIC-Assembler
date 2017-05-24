#ifndef _HELPER_
#define _HELPER_

#include "data.h"

//colors for terminal printing
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//for MessagesHandler function
//msg types
#define ERROR_ 0
#define WARNING_ 1
#define NOTE_ 2
//terminate or not
#define TERMINATE_ 1
#define NOTERMINATE_ 0
//parts IDs
#define MAIN_ 0
#define PASS1_ 1
#define PASS2_ 2
#define INPUTPARAMETERS_ 3

/*
 * add the second string to the first one
 * this function will use malloc to store the new string
 * this function do not free any string
 */
static inline char* strConcat(const char *s1, const char *s2);
/*
 * add the char to the string
 * this function will use malloc to store the new string
 */
static inline char* charConcat(char *s1, const char s2);
/*
 * create new empty string
 * this function will use malloc to store the new string
 * this function do not use free
 */
static inline char* emptyString();
/*
 * this function will return 1 incase the two string equal,
 * zero otherwise
 */
static inline int strEqual(const char *s1, const char *s2);
/*
 * this function will return 1 incase there is string in the given,
 * string or zero otherwise
 * this fuction will set outstart, outend to the string index starting
 * from zero
 */
static inline int locateString(const char *s1, int len, int* outstart, int* outend);
/*
 * this function will return capital char from small char
 * or it will return the same one
 */
static inline char bg(char chr);
/*
 * this function will return small char from captial char
 * or it will return the same one
 */
static inline char sml(char chr);
/*
 * this function will return capital string from small chars
 * this function will alter the given string 
 */
static inline char* sbg(char* str);
/*
 * this function will return small string from small chars
 * this function will alter the given string 
 */
static inline char* ssml(char* str);
/*
 * this function will return 1 incase the string is valid
 * hex number
 */
static inline int checkHex(char* hex);
/*
 * this function will return 1 incase the string is valid 
 * Positive hex number
 */
static inline int checkPosHex(char* hex);
/*
 * this function will return 1 incase the string is valid
 * number
 */
static inline int checkNum(char* hex);
/*
 * this function will return 1 incase the string is valid
 * number with or without leading sign
 */
static inline int checkSignNum(char* hex);
/*
 * this function will validate the given parameters from the users
 * and set the names of the files from which the programe will read
 * and write
 */
static inline void validateParameters(int argc, char* argv[],assemblerData* x);
/*
 * this function will how many the given long will take if it prints
 * as hex number
 */
static inline int offSet(long x);
/*
 * this function will how many the given long will take if it prints
 * as decimal number
 */
static inline int offSet10(long x);
/*
 * this function will handle the erros, warnings and Notes from the code
 * it's the only part allowed to user printf or terminate the code
 */
static inline void messagesHandler(message x, passData* y, int freeIt);
/*
 * create and retrun new Message
 */
static inline message newMessage(int type,int terminate, int from, char* toWrite);
/*
 * print empty line
 */
static inline void println();

#endif