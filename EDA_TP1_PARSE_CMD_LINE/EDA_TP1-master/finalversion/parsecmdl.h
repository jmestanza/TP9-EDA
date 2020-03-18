#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define ZERO "0"
#define END_OF_ARGUMENTS '0'
#define OPTION_IDENTIFIER '-'
#define PARSE_CMDL_ERROR -1

#define STOP 0

enum{NO,YES};
enum{OPTION,PARAM};

typedef int (*pCallback) (char *, char*, void *);

int parseCmdLine(int argc, char *argv[], pCallback p, void*userData);