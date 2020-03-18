#include "ParserFunc.h"

int parsecmdline(int argc, char* argv[], usercall userfun, void* userdata){
	int parsed=0;											//El TP pide cantidad de valores parseados correctamente.
	for(int i=1; i<argc; i++){
		if(argv[i][0]=='-'){								//Es key?
			if(argv[i][1]!=NULL){							//Verifico que haya nombre en la key. Error de forma 2.
				
				if((i+1) < argc){							//Verifico que haya Value. Error de forma 1.
					
					if(userfun(argv[i]+1,argv[i+1],userdata)==-1) return CBackErr;	//llamo al callback
					i++;									//Salteo el Value
					parsed+=1;								//Se parseo correctamente.
				}else{
					printf("Parse Error: Error Form 1: Option %d, >>%s<< value is missing.\n",i,argv[i]);
					return badValue;
				}
			}else{
				printf("Parse Error: Error Form 2: Option %d missing key name.\n",i);
				return badKey;
			}
		}else{												//Si hay algo que parsear y no es una key, es un parametro.
			
			if(userfun(NULL,argv[i],userdata)==-1) return CBackErr;
			parsed+=1;
		}
	}
	
	return parsed;
}
