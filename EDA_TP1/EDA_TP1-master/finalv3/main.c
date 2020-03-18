#include <stdio.h>
#include "parsecmdl.h"

/*EDA_TP1
Grupo: 2

Integrantes:

Larroque, Matías
Londero, Tomás
Mestanza, Joaquín
Mollón, Manuel


En este tp se pidió hacer un parseo de argumentos por linea de comando
siguiendo una estructura de -option value / parametro
En este parseo se pide que la funcion parsecmdline devuelva -1 si hubo un error,
a esto le añadimos la posibilidad de que nos informe en que opción o parametro falla
La función parsecmdline utiliza un callback para hacer la validación de los argumentos
por línea de comando. Como variante para este tp decidimos implementar que el callback
se fije si el string pasado por linea de comandos ademas de tener la estructura válida
que este efectivamente en una lista tanto de opciones, values y parametros.

*/


#define FALSE 0
#define TRUE 1
#define MAX_TEST_BAD_ARGS 10
#define NUM_BAD_ARG_TESTS 9
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


int main(int argc, char*argv[]){

	char *valid_cmd[]={"size","font","queue","color"}; // lista de opciones válidas
	char *valid_values[]={"arial","times","grow"};
	char *valid_param[]={"hello","bye"};	// lista de parametros válidos
	
	userdata_t user_info;
	
	user_info.cant_valid_options = sizeof(valid_cmd)/sizeof(valid_cmd[0]);
	user_info.cant_valid_params = sizeof(valid_param)/sizeof(valid_param[0]);
	user_info.cant_valid_values = sizeof(valid_values)/sizeof(valid_values[0]);


	user_info.p2valid_cmd = &valid_cmd;
	user_info.p2valid_value = &valid_values;
	user_info.p2valid_param = &valid_param;
	
	pCallback p=parseCallback;


//  printf para cuando parsecmdline es usado con la consola
//	printf("%d\n",parseCmdLine(argc, argv, p, &user_info) );

/*BANCO DE PRUEBAS:
Aclaración: 
tenemos listas de opciones, parametros y values válidos que por lo pronto
solo se verifica que esten en la lista, no que las opciones y los values sean
compatibles entre sí.
Se añade como ultimo elemento un puntero a null simulando el puntero a null
que nos da argv[argc]
*/


										   	    
// Casos buenos : 

    char *good_test1[]={"miprog","-size","grow",NULL}; // option valida y value valido
	char *good_test2[]={"miprog","-size", "grow", "-font","arial",NULL};
	char *good_test3[]={"miprog","hello",NULL}; // parametro valido
	
	printf("%d\n",parseCmdLine( (sizeof(good_test1)/sizeof(good_test1[0])) -1, good_test1,p, &user_info ));
	printf("%d\n",parseCmdLine( (sizeof(good_test2)/sizeof(good_test1[0])) -1, good_test2,p, &user_info ));
	printf("%d\n",parseCmdLine( (sizeof(good_test3)/sizeof(good_test1[0])) -1, good_test3,p, &user_info ));

// Casos malos:
	char *matriz_prueba[NUM_BAD_ARG_TESTS][MAX_TEST_BAD_ARGS] ={{"miprog", "-", NULL}, // - sin option ni value
												{"miprog","-asd", NULL}, // option no valida sin value
												{"miprog","-size",NULL},// option valida pero sin value
											    {"miprog","-size","asd",NULL}, // option valida pero con value invalido
											 	
											    {"miprog","-size","grow", "-font","asd",NULL}, // primer option valida 2da option valida y value no valido
											    {"miprog","-size","grow", "-asd","arial",NULL}, // option valida y segunda option no valida


												{"miprog","asd",NULL}, //parametro no valido
												{"miprog","hello","asd",NULL}, //parametro 1 valido , parametro 2 no valido
												{"miprog","hello","-size",NULL}}; // parametro valido, opcion valida pero sin value (al final)


	int i;
    for (i = 0; i < NUM_BAD_ARG_TESTS; i++){
        printf("prueba %d:  resultado:%d \n", i+1,parseCmdLine(MAX_TEST_BAD_ARGS, matriz_prueba[i], p, &user_info));
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

			noerror=validate_letterstr(value,userData,VALUE);

		}
	
	}else{ // errores correspondientes a parametros

		noerror=validate_letterstr(value,userData,PARAM);	

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
