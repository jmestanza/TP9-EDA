#include <stdio.h>
#include "parsecmdl.h"

int main(int argc, char*argv[]){

	char *valid_cmd[CANT_VALID_CMD]={VALID_CMD1,VALID_CMD2};
	char *valid_param[CANT_VALID_PARAMS]={VALID_PARAM1,VALID_PARAM2};
	
	userdata_t user_info;
	user_info.p2valid_cmd = &valid_cmd;
	user_info.p2valid_param = &valid_param;
	pCallback p=parseCallback;

	printf("%d\n",parseCmdLine(argc, argv, p, &user_info) );

}
