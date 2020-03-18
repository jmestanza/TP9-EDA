
#include "main.h"

uint32_t mini(uint32_t a, uint32_t b){
	return a < b ? a : b;
}

uint32_t maxi(uint32_t a, uint32_t b) {
	return a > b ? a : b;
}


#define DEFAULT_W 5
#define DEFAULT_H 5
#define DEFAULT_C_ROBS 1

typedef pos_t<double>* position_list;

int main(int argc, char** argv) {
	srand(time(NULL));
	int error = OFF;
	userDataType Flags={0,0,0,0,0};
	userDataType uData={DEMO,&Flags,DEFAULT_W,DEFAULT_H,DEFAULT_C_ROBS};
	if(parsecmdline(argc,argv,&Callback,&uData)!=-1){
		if(!CheckParameters(&uData)){
			if (al_init()) {
				if(al_init_image_addon()){
					if(al_init_primitives_addon()){
						ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
						ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
						ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
						
						if (display && event_queue && timer) {
							al_register_event_source(event_queue, al_get_display_event_source(display));
							al_register_event_source(event_queue, al_get_timer_event_source(timer));
							al_clear_to_color(al_map_rgb(0, 0, 0));
							
							if(uData.mode==REALTIME){
								if(real_time_sim(&uData, display, event_queue, timer)) //llama a la funcion
									error=ON; //en caso de fallar devuelve error
							}
							if(uData.mode==FULLSIM){
								if(full_simulation(&uData, display, event_queue, timer))
									error=ON;
							}

							al_destroy_event_queue(event_queue);
							al_destroy_timer(timer);
							al_destroy_display(display);						
						}else{
							printf("Failed to start allegro. \n");
							al_destroy_display(display);
							al_destroy_event_queue(event_queue);
							al_destroy_timer(timer);
							error=ON;
						}
						al_shutdown_primitives_addon();
					}else{ 
						printf("Failed to initialize allegro primitives addon. \n");
						
						return ERR;
					}al_shutdown_image_addon();
				}else{
					printf("Failed to initialize image addon. \n");
					return ERR;
				}al_uninstall_system();
			}else{
				printf("Failed to initialize allegro. \n");
				return ERR;
			}
		}else{
			error=ON;
		}
		
	}else{
		error=ON;
	}

	if(error) system("PAUSE");			//En caso de error frena el programa para leer el mensaje de error.
	
	if(error == ON)	return ERR;
	else return 0;
}

int real_time_sim(userDataType* uData, ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer){
	pos_t<uint32_t>screen_data; screen_data.x = SCREEN_WIDTH; screen_data.y = SCREEN_HEIGHT;
	pos_t<double> scr; scr.x = SCREEN_WIDTH; scr.y = SCREEN_HEIGHT;
	pos_t<uint32_t> sim_data; sim_data.x=uData->width; sim_data.y=uData->heigh;
	
	model_t *test = model_init(uData->cRobots, uData->width, uData->heigh);

	pos_t<double> *positions = model_get_all_positions(test);
	graphic_simluation_t *sim = sim_create(&screen_data, &sim_data, PERIOD, uData->cRobots,positions);
	free(positions);
	model_update_cycle(test);
	/*** create graphic simulation ***/

		if (sim == NULL) {
			printf("Failed to create graphic simulation\n");
			return -1;
		}

		model_update_cycle(test);
		position_list list = model_get_all_positions(test);
		sim_update_robot_pos(sim, list);

		free(positions);

		
		int exit = 0;
		printf("Starting ...\n");
		

		uint32_t cnt = 0;

		while (!exit) {
			ALLEGRO_EVENT ev;
			if (al_get_next_event(event_queue, &ev)) {
				if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
					exit = 1;
				}
			}
			al_clear_to_color(al_map_rgb(255, 255, 255));
			sim_draw(sim);

			bool next = sim_advance_animation(sim);
			
			if (next) {
				
				model_update_cycle(test);
				update_simulation_from_model(test, sim);
				
			}

			al_flip_display();
			al_rest(0.03);

			if (model_end(test)) {
				al_rest(0.5);
				model_reset(test,uData->cRobots);
				cnt++;
				printf("Restart %d\n",cnt);
			}
		}

		sim_destroy(sim);
		model_destroy(test);
	return 0;
	
}

int full_simulation(userDataType* uData, ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer){
	pos_t<uint32_t>screen_data; screen_data.x = SCREEN_WIDTH; screen_data.y = SCREEN_HEIGHT;
	pos_t<double> scr; scr.x = SCREEN_WIDTH; scr.y = SCREEN_HEIGHT;
	pos_t<uint32_t> sim_data; sim_data.x=uData->width; sim_data.y=uData->heigh;
	model_t *test = model_init(uData->cRobots, uData->width, uData->heigh);

	bool found = false;
		double result[100];
		for (int k = 0; k < 50; k++) {
			result[k] = 0.0;
		}
		uint64_t temp;
		double time_passed = 0;
		
		
		for (uint32_t r = 1; r < 50 && !found; r++){
			
			model_t *test = model_init(r, sim_data.x, sim_data.y);

			temp = 0;
			uint32_t vmax = 0;
			uint32_t vmin = UINT32_MAX;
			

			for (int j = 0; j < 1000; j++) {
				
				int exit = 0;
				model_reset(test,r);
				while (!exit) {
					model_update_cycle(test);
					if (model_end(test)) {
						exit = 1;
					}
				} // while de no exit
				temp += test->tick_count;
				vmax = maxi(test->tick_count, vmax);
				vmin = mini(test->tick_count, vmin);	
			} // for de j
			

			printf("r:%lu temp :%lf (err = %f) \n",r,temp/1000.0,(((vmax-vmin)/2.0) * 100) / temp );
			
			result[r-1] = temp / 1000.0;
			
			if (r>2 && ((result[r - 2] - result[r-1]) <= 0.1)) {
				found = 1;
			}
		}
		model_destroy(test);

		
		
		if (printHistogram(result, &scr, 50)) {
			return -1;
		}
	system("pause");
	return 0;
}

void set_positions(model_t *model, graphic_simluation_t *sim) {
	pos_t<double> *positions = (pos_t<double> *)malloc(model->robot_cnt * sizeof(pos_t<double>));
	
	for (int i = 0; i < (int)(model->robot_cnt); i++) {
		
		positions[i] = robot_get_pos(model->robots[i]);
	}

}
void update_simulation_from_model(model_t *model, graphic_simluation_t *sim) {
	position_list list = model_get_all_positions(model);
	sim_update_robot_pos(sim, list);
	sim_update_tiles(sim, model->piso->baldosas);
}

int	Callback(char *key, char *value, void *userData){
	int errfl = ON;										//Dado que no se aceptan más de una misma llamada a un parametro, se utiliza un flag 
	userDataType *uData=(userDataType*)userData;		//para verificar que el parametro no fue llamado anteriormente.

	if(key == NULL){	
		//if(!strcmp(value,"help")){		//Possible feature
		//	help();								
		//	return -1;
		//}else{
			printf("Error: Program only accepts \"-key value\" type arguments.\n");
			return ERR;
		//}
	}

	if(!strcmp(key,"Modo")){				//Para todo argumento posible,
		if(uData->Flags->mode == OFF){		//si lo encuentra enciende el flag,
			if(!strcmp(value,"1")){		//y guarda el valor para ser verificado más adelante
				uData->mode = REALTIME;			//en otra funcion.
				uData->Flags->mode = ON;
			}else
			if(!strcmp(value,"2")){
				uData->mode = FULLSIM;
				uData->Flags->mode = ON;
			}else{
				printf("Error: %s is not a valid mode. Only two modes are '1' (Real time) and '2' (Full simulation).",value);
				return ERR;
			}
			errfl = OFF;
		}else{
			printf("Error: Multiple -%s arguments!\n",key); //Y para todo argumento posible,
			return ERR;										//si esta repetido se reporta como error.
		}
	}
	
	if(!strcmp(key,"Width")){
		if(uData->Flags->width == OFF){
			uData->width = strtol(value,NULL,10);
			uData->Flags->width = ON;
			errfl = OFF;
		}else{
			printf("Error: Multiple -%s arguments!\n",key);
			return ERR;
		}
	}

	if(!strcmp(key,"Heigh")){
		if(uData->Flags->heigh == OFF){
			uData->heigh = strtol(value,NULL,10);
			uData->Flags->heigh = ON;
			errfl = OFF;
		}else{
			printf("Error: Multiple -%s arguments!\n",key);
			return ERR;
		}
	}

	if(!strcmp(key,"Robots")){
		if(uData->Flags->cRobots == OFF){
			uData->cRobots = strtol(value,NULL,10);
			uData->Flags->cRobots = ON;
			errfl = OFF;
		}else{
			printf("Error: Multiple -%s arguments!\n",key);
			return ERR;
		}
	}
	
	if(errfl == ON){
		printf("Error: Invalid key -%s!\n",key);
		return (ERR);
	}
	return 0;
}

int	CheckParameters(userDataType *uData){
	if(uData->Flags->mode==OFF){
		if((uData->Flags->cRobots==ON) || (uData->Flags->heigh==ON) || (uData->Flags->width==ON)){
			printf("Error: Please specify mode when selecting parameters. Call with no parameters to engage demo mode.\n");
			return ERR;
		}else{ //Modo demostrativo utiliza parametros por default.
			printf("WARNING: No mode selected; Demo mode engaged. All parameters default. Showcasing real time.\n");
			printf("Mode: Demo  NºRobots: %d  Floor Heigh: %d  Floor Width: %d  NºTiles: %d\n",uData->cRobots,uData->heigh,uData->width,uData->heigh*uData->width);
			uData->mode=REALTIME;
			return 0;
		}
	}

	//If a value was chosen, check its validity.

	if(uData->Flags->cRobots == ON){
		if( (uData->cRobots <= 0) || (uData->cRobots > (uData->heigh*uData->width)) ){
			printf("Error: Robot quantity out of bounds. There cannot be more robots than tiles (heigh*width) and there must be robots on screen. Value chosen: %d Amount of tiles: %d\n",uData->cRobots,uData->heigh*uData->width);
			return ERR;
		}
	}else{
		printf("Warning: No robot quantity chosen. Defaulted to 1.\n");
	}

	if(uData->Flags->heigh == ON){
		if( (uData->heigh <= 0) || (uData->heigh > 100) ){
			printf("Error: Heigh out of bounds. Range for heigh: [1...100] Value chosen: %d \n",uData->heigh);
			return ERR;
		}
	}else{
		printf("Warning: No heigh chosen. Defaulted to 5.\n");
	}

	if(uData->Flags->width == ON){
		if( (uData->width <= 0) || (uData->width > 70) ){
			printf("Error: Width out of bounds. Range for width: [1...70] Value chosen: %d \n",uData->width);
			return ERR;
		}
	}else{
		printf("Warning: No width chosen. Defaulted to 5.\n");
	}
	
	//If there was no error, print the chosen values.
	
	if(uData->mode == REALTIME){	//Modo tiempo real
		printf("Mode: Real Time Simulation  NºRobots: %d  Floor Heigh: %d  Floor Width: %d  NºTiles: %d\n",uData->cRobots,uData->heigh,uData->width,uData->heigh*uData->width);
	}
	if(uData->mode == FULLSIM){	//Modo simulacion completa estadistica
		printf("Mode: Full Simulation  NºRobots: %d  Floor Heigh: %d  Floor Width: %d  NºTiles: %d\n",uData->cRobots,uData->heigh,uData->width,uData->heigh*uData->width);
	}
			



	return 0;
}

