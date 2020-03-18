#include "parsecmdl.h"

int parseCmdLine(int argc, char *argv[], pCallback p, void*userData){

	int cant_options=0; // init de contadores
	int cant_params=0;

	int parseerror=0; 
	int parsing_ans;
	int i;
	
	for(i=1;i<argc && (parseerror!=PARSE_CMDL_ERROR);i++){ //skipeo el nombre del ejecutable 
		if(argv[i][0]==OPTION_IDENTIFIER){
	
				if(p(argv[i],argv[i+1],userData)){
					i++; // avanzamos el indice ya que una opcion tiene 2 strings
					cant_options++;
				}else{
					parseerror= PARSE_CMDL_ERROR;
				}

		}else{ // si el formato no es -option value es un parametro
	
			if(p(NULL,argv[i],userData)){
				cant_params++;	
			}else{
				parseerror= PARSE_CMDL_ERROR;
			}
		}
	}
	if(parseerror==PARSE_CMDL_ERROR){
		parsing_ans=PARSE_CMDL_ERROR;
	}else{
		parsing_ans= cant_params + cant_options;
	}
	return parsing_ans;
}


