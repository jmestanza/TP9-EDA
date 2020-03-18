#ifndef PARSER_H
#define PARSER_H

	#include <stdio.h>
	#define badKey -1
	#define badValue -1
	#define CBackErr -1

	typedef int (*usercall)(char*,char*,void*);

	int parsecmdline(int argc, char *argv[], int(*uCallback)(char*, char*, void*), void *uData);

	//typedef enum{NoError, CBackErr, badKey, badValue} errorType; For future use.

#endif


/**
*	Command Line Parser:
*
*			This function, as the name suggests, reads the data from the execution arguments. Firstly it sorts
*		the arguments by "-key" followed by a "value" or a parameter without "-". Then, it delivers them along 
*		with a struct to a callback for interpreting.
*	
*	Luciano Castrogiovanni
**/

