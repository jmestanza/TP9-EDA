#include <stdio.h>
#include "parsecmdl.h"
int main(int argc, char*argv[]){

// nota: tratar de acceder a argv[argc] tira SEGMENTATION FAULT

	userdata_t user_info;
	user_info.cant_args = argc;	
	pCallback p=parseCallback;

	printf("%d\n",parseCmdLine(argc, argv, p, &user_info) );

}
