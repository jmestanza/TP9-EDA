#include "parsecmdl.h"

int parseCmdLine(int argc, char *argv[], pCallback p, void*userData){

	int cant_options=0; // init de contadores
	int cant_params=0;
	int parseerror; 
	int status=0;
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
		status=PARSE_CMDL_ERROR;
	}else{
		status= cant_params + cant_options;
	}
	return status;
}


