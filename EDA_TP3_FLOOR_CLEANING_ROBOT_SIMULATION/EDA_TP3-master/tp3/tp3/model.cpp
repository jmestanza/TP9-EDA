

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "model.h"
#include "utils.h"

const double PI = 2*acos(0);
const int SENSIBILITY_CONST = 1000;

bool valid_move(pos_t<double> *pos, double angle,pos_t<uint32_t> *limits) {
	double new_x = pos->x + cos(angle * PI / 180.0);
	double new_y = pos->y + sin(angle * PI / 180.0);

	if (new_x >= 0 && new_x <= (double)limits->x){
		if (new_y >= 0 && new_y <= (double)limits->y){
			return 1;
		}
	}

	return 0;
}

void move(pos_t<double> *pos, double angle) {
	pos->x += cos(angle * PI / 180.0);
	pos->y += sin(angle * PI / 180.0);
}


robot_t *robot_init(pos_t<double> robot_p ,double ang, pos_t<uint32_t> *model_size_d) {
	robot_t *robot = (robot_t*)malloc(sizeof(robot_t));
	if (robot == NULL) return NULL;
	
	robot->model_size = model_size_d;
	robot->pos = robot_p;
	robot->angle = ang;
	
	return robot;
}


void robot_move(robot_t *robot) {
	double total_size_x = (double)robot->model_size->x;
	double total_size_y = (double)robot->model_size->y;
	
	if (valid_move(&robot->pos,robot->angle,robot->model_size)){
		move(&robot->pos, robot->angle);
	}else{  ///  we need to generate a new direction
		bool found = 0;
		while (!found) {
			double dir = (double)(rand() % 360);
			
			if (valid_move(&robot->pos, dir, robot->model_size)) {
				move(&robot->pos, dir);
				robot->angle = dir;
				found = 1;
			}
		}
	}

	//printf("%f \n", dir);
	//printf("%f %f \n", robot->pos.x, robot->pos.y);
}
pos_t<double> robot_get_pos(robot_t *robot) {
	return robot->pos;
}
void robot_show(robot_t *robot) {
	printf("%0.4f %0.4f (%0.4f) \n",robot->pos.x, robot->pos.y, robot->angle);
}
void robot_destroy(robot_t *robot) {
	free(robot);
}

piso_t* piso_init(pos_t<uint32_t> *msize) {
	piso_t *piso = (piso_t*)malloc(sizeof(piso_t));
	if (piso == NULL) return NULL;

	piso->model_size = msize;
	piso->baldosas = (bool*)malloc(sizeof(bool)*piso->model_size->y*piso->model_size->x);
	if (piso->baldosas == NULL) return NULL;

	for (int i = 0; i < (int)piso->model_size->y; i++) {
		for (int j = 0; j < (int)piso->model_size->x; j++) {
			piso->baldosas[i*piso->model_size->x + j] = 0;
		}
	}
	piso->limpias = 0;
}
void piso_set_baldosa(piso_t* piso,uint32_t i, uint32_t j, uint32_t value) {
	uint32_t old_value = piso->baldosas[i*piso->model_size->x + j], new_value = value;
	piso->baldosas[i*piso->model_size->x + j] = value;
	uint32_t delta = value - old_value;
	piso->limpias += delta;
}
bool piso_get_baldosa(piso_t *piso,uint32_t i, uint32_t j) {
	return piso->baldosas[i*piso->model_size->x + j];
}
void piso_destroy(piso_t *piso) {
	free(piso->baldosas);
	free(piso);
}

model_t* model_init(uint32_t robot_cnt, uint32_t width, uint32_t height) {
	model_t *model = (model_t*)malloc(sizeof(model_t));
	if (model == NULL) return NULL;

	model->model_size.x = width; model->model_size.y = height;

	piso_t *piso = piso_init(&model->model_size);
	if (piso == NULL) return NULL;
	model->piso = piso;
	model->robot_cnt = robot_cnt;
	model->configured_robots = robot_cnt;
	model->tick_count = 0;
	model->robots = (robot_t**)malloc(sizeof(robot_t*)*robot_cnt);

	//printf("%lu \n", sizeof(model->robots));

	if (!model->robots) return NULL;

	for (int i = 0; i < (int)robot_cnt; i++) {
		pos_t<double> position; 
		position.x = db_prng((int)model->model_size.x , SENSIBILITY_CONST);
		position.y = db_prng((int)model->model_size.y , SENSIBILITY_CONST);
		double angle = db_prng((int)360.0, SENSIBILITY_CONST);
		model->robots[i] = robot_init(position,angle,&model->model_size);
		
	}
	
	return model;
}
void model_reset(model_t* model,uint32_t active_robots) {

	model->tick_count = 0;
	if (active_robots > model->configured_robots) {
		printf("Wrong model reset parameters");
		return;
	}
	model->robot_cnt = active_robots;
	//printf("%lu \n", sizeof(model->robots));
	//model->robot_cnt = active_robots;
	for (int i = 0; i < (int)model->robot_cnt; i++) {
		pos_t<double> position;
		position.x = db_prng((int)model->model_size.x, SENSIBILITY_CONST);
		position.y = db_prng((int)model->model_size.y, SENSIBILITY_CONST);
		double angle = db_prng((int)360.0, SENSIBILITY_CONST);
		model->robots[i] = robot_init(position, angle, &model->model_size);


	}
	model->piso->limpias = 0;
	for (int i = 0; i < (int)model->piso->model_size->y; i++) {
		for (int j = 0; j < (int)model->piso->model_size->x; j++) {
			model->piso->baldosas[i*model->piso->model_size->x + j] = 0;
		}
	}
}
bool model_end(model_t *model) {
	return (model->piso->limpias >= model->model_size.x * model->model_size.y);
}

void model_update_cycle(model_t * model) {
	
	for (int i = 0; i < (int)model->robot_cnt; i++) {
		robot_move(model->robots[i]);
		pos_t<double> position = robot_get_pos(model->robots[i]);

		piso_set_baldosa(model->piso, floor(position.y), floor(position.x),1 );
	}
	model->tick_count++;
	//printf("%d \n", model->piso->limpias);
}
void model_full_simulation(model_t * model) {

}
void model_show_robots(model_t *model) {
	printf("------------------------\n");
	printf("Showing model robots data: \n");
	for (int i = 0; i < (int)model->robot_cnt; i++) {
		robot_show(model->robots[i]);
	}
	printf("------------------------\n");
}
pos_t<double> * model_get_all_positions(model_t *model) {
	pos_t<double> *positions = (pos_t<double>*)malloc(sizeof(pos_t<double>)*model->robot_cnt);
	
	for (int i = 0; i < (int)model->robot_cnt; i++) {
		positions[i] = robot_get_pos(model->robots[i]);
		//printf("%f %f \n", positions[i].x, positions[i].y);
	}
	return positions;
}
void model_destroy(model_t * model) {
	piso_destroy(model->piso);
	for (int i = 0; i < (int)model->robot_cnt; i++) {
		robot_destroy(model->robots[i]);
	}
	free(model->robots);
	free(model);
}