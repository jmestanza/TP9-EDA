#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define ZERO "0"
#define END_OF_ARGUMENTS '0'
#define OPTION_IDENTIFIER '-'
#define PARSE_CMDL_ERROR -1
#define CANT_VALID_CMD 2
#define CANT_VALID_PARAMS 2
#define VALID_CMD1 "size"
#define VALID_CMD2 "font"
#define STOP 0
#define MIN_VALUE 0
#define MAX_VALUE 500
#define VALID_PARAM1 "hello"
#define VALID_PARAM2 "bye"

enum{NO,YES};
enum{OPTION,PARAM};

typedef int (*pCallback) (char *, char*, void *);
typedef struct{		
	char *(*p2valid_cmd)[CANT_VALID_CMD];
	char *(*p2valid_param)[CANT_VALID_PARAMS];
					
}userdata_t;

int parseCallback(char *key, char *value, void *userData);
int parseCmdLine(int argc, char *argv[], pCallback p, void*userData);
int validate_letterstr(char *str_to_cmp, void *userData, int kindofop);
int is_only_number_str(char * value);
int validate_numstr(char * value);