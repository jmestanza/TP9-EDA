#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OPTION_IDENTIFIER '-'
#define PARSE_CMDL_ERROR -1

enum{NO,YES};
enum{OPTION,VALUE,PARAM};

typedef int (*pCallback) (char *, char*, void *);

int parseCmdLine(int argc, char *argv[], pCallback p, void*userData);