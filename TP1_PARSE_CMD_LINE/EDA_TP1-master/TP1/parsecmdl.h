#include <stdio.h>
#define FALSE 0
#define TRUE 1
#define ZERO "0"
#define END_OF_ARGUMENTS '0'
#define OPTION_IDENTIFIER '-'
#define PARSE_CMDL_ERROR -1


typedef int (*pCallback) (char *, char*, void *);
typedef struct{
	int index;		
	char* num;
	int cant_args;				
}userdata_t;

int parseCallback(char *key, char *value, void *userData);
int parseCmdLine(int argc, char *argv[], pCallback p, void*userData);
