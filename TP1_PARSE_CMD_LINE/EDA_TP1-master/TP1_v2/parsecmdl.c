#include "parsecmdl.h"

int parseCmdLine(int argc, char *argv[], pCallback p, void*userData){

	int cant_options=0; // init de contadores
	int cant_params=0;

	userdata_t * p2userdata= (userdata_t*) userData;
	int i;
	for(i=1;i<argc;i++){ //skipeo el nombre del ejecutable 
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
		if(!value){ // si es opcion y es el ultimo argumento -> error
			noerror=FALSE;
		}
		if(noerror){
			noerror=validate_letterstr(key+1,userData,OPTION);
			// evadimos el - 
		}

		if(noerror){ 

		if(is_only_number_str(value)){ // era de solo numeros

			noerror=validate_numstr(value);
		 
		}else{ // combinacion de letras y numeros
			noerror=validate_letterstr(value,userData,OPTION);
			}	
		}


	}else{ // errores correspondientes a parametros
	if(is_only_number_str(value)){
			noerror=FALSE; // no puede haber param con solo numeros
		 
		}else{ // combinacion de letras y numeros 
			noerror=validate_letterstr(value,userData,PARAM);
			}	
		}
	return noerror;
}


int validate_letterstr(char *str_to_cmp, void *userData, int kindofop){
	int noerrorsofar;
	userdata_t *p2udata = (userdata_t*)userData; 
	int i;
	if(kindofop){
		i=CANT_VALID_CMD;	
	}else{
		i=CANT_VALID_PARAMS;
	}
	noerrorsofar = FALSE;
	while(i--){
		if(kindofop==OPTION){
			if(!strcmp(str_to_cmp, *(*(p2udata->p2valid_cmd)+i))){ 
				i=STOP;
				noerrorsofar=TRUE;	
			}
		}else{
			if(!strcmp(str_to_cmp, *(*(p2udata->p2valid_param)+i))){ 
				i=STOP;
				noerrorsofar=TRUE;	
			}
		}
	}
	return noerrorsofar;
}


int is_only_number_str(char * value){
	int n=strlen(value); int t; int num=0; int ans;

	for(t=0;t<n;t++){
		if(value[t]>='0' && value[t]<='9'){
			num++;
		}
	}
	if(num==n){
		ans=YES;
	}else{
		ans=NO;
	}
	return ans;
}
int validate_numstr(char * value){
	int noerrorsofar;
	int num_str=atoi(value);
	if(MIN_VALUE <= num_str && num_str <= MAX_VALUE){
		noerrorsofar=TRUE;
	}else{
		noerrorsofar=FALSE;
	}
	return noerrorsofar;
}	