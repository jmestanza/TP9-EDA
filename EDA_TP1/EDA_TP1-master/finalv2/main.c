#include <stdio.h>
#include "parsecmdl.h"

#define FALSE 0
#define TRUE 1
#define MIN_VALUE 0
#define MAX_VALUE 500
#define NUM_TESTS 2
#define NUM_ELEM 6
#define STOP 0


typedef struct{		
	int cant_valid_options;
	int cant_valid_params;
	int cant_valid_values;

	char *(*p2valid_cmd)[];
	char *(*p2valid_param)[];
	char *(*p2valid_value)[];
					
}userdata_t;

int parseCallback(char *key, char *value, void *userData);
/*Funciones que utiliza el callback*/
int validate_letterstr(char *str_to_cmp, void *userData, int kindofop);
// validate_letterstr devuelve 1 si el string pertenece a la lista de opciones/parametros validos sino 0
int is_only_number_str(char * value);
// is_only_number_str devuelve 1 si el string es solo de numeros, caso contrario devuelve 0 
int validate_numstr(char * value);
// validate_numstr devuelve 1 si el string de numeros esta en el rango indicado por las macros
// MIN_VALUE, MAX_VALUE si no lo esta devuelve 0



int main(int argc, char*argv[]){

	char *valid_cmd[]={"size","font","queue","color"}; // lista de opciones válidas
	char *valid_param[]={"hello","bye"};	// lista de parametros válidos
	char *valid_values[]={"arial","times","grow"};

	userdata_t user_info;
	
	user_info.cant_valid_options = sizeof(valid_cmd)/sizeof(valid_cmd[0]);
	user_info.cant_valid_params = sizeof(valid_param)/sizeof(valid_param[0]);
	user_info.cant_valid_values = sizeof(valid_values)/sizeof(valid_values[0]);


	user_info.p2valid_cmd = &valid_cmd;
	user_info.p2valid_param = &valid_param;
	user_info.p2valid_value = &valid_values;
	
	pCallback p=parseCallback;


//  este printf es para cuando parsecmdline es usado con la consola
//	printf("%d\n",parseCmdLine(argc, argv, p, &user_info) );

//BANCO DE PRUEBAS
	/*Aclaracion: 
	tenemos listas de opciones, parametros y values válidos que por lo pronto
	solo se verifica que esten en la lista, no que las opciones y los values sean
	compatibles entre sí.
	Para values podemos tener dos casos: solo numero y combinación de numero-letra
	si es combinacion me fijo en la lista sino tenemos definidas macros 
	MIN_VALUE y MAX_VALUE lo cual nos dan un intervalo fijo de numeros que son validos
	*/

	char *matriz_prueba[NUM_TESTS][NUM_ELEM] = {{"miprog", "bye", "-size", "123", "hello", NULL}, {"mi_prog", "0", "-key_2", "30", "--", NULL}};
	int i; int test;
    for (i = 0; i < NUM_TESTS; i++){
        printf("prueba %d:  resultado:%d \n", i+1,parseCmdLine(NUM_ELEM-1, matriz_prueba[i], p, &user_info));
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
			// evadimos el OPTION_IDENTIFIER
		}

		if(noerror){ 

		if(is_only_number_str(value)){ // era de solo numeros

			noerror=validate_numstr(value);
		 
		}else{ // combinacion de letras y numeros
			noerror=validate_letterstr(value,userData,VALUE);
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
	if(kindofop==OPTION){ // si no es una opcion se da por sentado que es un parametro
		i=p2udata->cant_valid_options;
		
	}else if(kindofop==VALUE){
		i=p2udata->cant_valid_values;
	}else if(kindofop==PARAM){
		i=p2udata->cant_valid_params;
	}
	noerrorsofar = FALSE;
	
	if(kindofop==OPTION){
		while(i--){
			if(!strcmp(str_to_cmp, *(*(p2udata->p2valid_cmd)+i))){ 
				i=STOP;
				noerrorsofar=TRUE;	
			}
		}
	}else if(kindofop==VALUE){
		while(i--){
			if(!strcmp(str_to_cmp, *(*(p2udata->p2valid_value)+i))){ 
				i=STOP;
				noerrorsofar=TRUE;	
			}
		}
	}else if(kindofop==PARAM){
		while(i--){
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