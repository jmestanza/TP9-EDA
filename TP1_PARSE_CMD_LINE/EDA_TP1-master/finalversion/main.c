#include <stdio.h>
#include "parsecmdl.h"
#define MIN_VALUE 0
#define MAX_VALUE 500
#define CANT_VALID_CMD 3
#define CANT_VALID_PARAMS 2


#define No_PRUEBAS 4
#define No_ELEMENTOS 6


typedef struct{		
	char *(*p2valid_cmd)[CANT_VALID_CMD];
	char *(*p2valid_param)[CANT_VALID_PARAMS];
					
}userdata_t;




int parseCallback(char *key, char *value, void *userData);
int validate_letterstr(char *str_to_cmp, void *userData, int kindofop);
int is_only_number_str(char * value);
int validate_numstr(char * value);

int main(int argc, char*argv[]){

	char *valid_cmd[CANT_VALID_CMD]={"size","font","mati"};
	char *valid_param[CANT_VALID_PARAMS]={"asd","hello"};
	
	userdata_t user_info;
	user_info.p2valid_cmd = &valid_cmd;
	user_info.p2valid_param = &valid_param;
	pCallback p=parseCallback;

	printf("%d\n",parseCmdLine(argc, argv, p, &user_info) );

	char *matriz_prueba[No_PRUEBAS][No_ELEMENTOS] = {{"miprog", "asd", "-size", "123", "hello", NULL}, {"mi_prog", "0", "-key_2", "30", "--", NULL},
													{"mi_prog", "-key_2", "30", "-f", NULL}, {"mi_prog", "asd", "-", "13", NULL}};
	int i;    
	int resul;
    for (i = 0; i < No_PRUEBAS; i++){
        printf("prueba %d:\n", i+1);
        if (resul = parseCmdLine(No_ELEMENTOS-1, matriz_prueba[i], p, &user_info)){   //desventaja del banco de pruebas: numero de elementos fijos
            printf("return: %d\n", resul);
        }
        else
        {
            printf("ERROR\n");
        }
    }




    return 0;

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