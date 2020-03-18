#include "parsecmdl.h"

int parseCmdLine(int argc, char *argv[], pCallback p, void*userData){

	int cant_options=0; // init de contadores
	int cant_params=0;

	userdata_t * p2userdata= (userdata_t*) userData;

	for(int i=1;i<argc;i++){ //skipeo el nombre del ejecutable 

		p2userdata->index=i;

		if(argv[i][0]==OPTION_IDENTIFIER){
	
				if(p(argv[i],argv[i+1],userData)){
					i++; // avanzamos el indice ya que una opcion tiene 2 strings
					cant_options++;
				}else{
					return PARSE_CMDL_ERROR;
				}

		}else{ // si el formato no es -option value es un parametro
	
			if(p(NULL,argv[i],userData)){
				cant_params++;	
			}else{
				return PARSE_CMDL_ERROR;
			}
		}
	}
		return cant_params+cant_options;
}


int parseCallback(char *key, char *value, void *userData){
	int noerror=TRUE;
	userdata_t * p2userinfo= (userdata_t*) userData;
	if(key){ // errores correspondientes a opciones
		
		if((p2userinfo->index==((p2userinfo->cant_args)-1))){
			noerror=FALSE;
		}
	// este es el caso en el que -f no tiene nada adelante
	// Si estoy en opciones y ademas en el ultimo argumento -> error 
	}
	
	/*si hay más casos añadir aca abajo:
	por ejemplo: lista de comandos validos*/

	return noerror;
}