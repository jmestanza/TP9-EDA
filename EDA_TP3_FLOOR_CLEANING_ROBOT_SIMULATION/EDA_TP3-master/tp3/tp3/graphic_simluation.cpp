
#include <math.h>
#include <algorithm>
#include <stdio.h>
#include <iostream>
#include "graphic_simulation.h"
#include "allegro5\allegro_image.h"

using namespace std;

const double ROBOT_FACTOR_SIZE_CONST = 0.5;


bitmaps_t* bitmaps_init() {
	bitmaps_t *bitmaps = (bitmaps_t*)malloc(sizeof(bitmaps_t));
	if (bitmaps == NULL) return NULL;

	bitmaps->dirty_tile = al_load_bitmap("sucia.png" );
	bitmaps->clean_tile = al_load_bitmap("limpia.png");
	bitmaps->robot      = al_load_bitmap("robot.png" );
	bitmaps->arrow      = al_load_bitmap("arrow.png" );

	if (bitmaps->dirty_tile == NULL) return NULL;
	if (bitmaps->clean_tile == NULL) return NULL;
	if (bitmaps->robot      == NULL) return NULL;
	if (bitmaps->arrow      == NULL) return NULL;

	return bitmaps;
}
void bitmaps_destroy(bitmaps_t * bitmaps) {
	al_destroy_bitmap(bitmaps->dirty_tile);
	al_destroy_bitmap(bitmaps->clean_tile);
	al_destroy_bitmap(bitmaps->robot);
	al_destroy_bitmap(bitmaps->arrow);

	free(bitmaps);
}


sim_robot_t * sim_robot_init(double speed,sim_data *data,pos_t<double> initial_pos){
	sim_robot_t * robot = (sim_robot_t *)malloc(sizeof(sim_robot_t));
	if (robot == NULL) return NULL;

	robot->target_position.x  = initial_pos.x;
	robot->target_position.y  = initial_pos.y;

	robot->start_position.y = initial_pos.y;
	robot->start_position.x = initial_pos.x;

	robot->current_position.y = initial_pos.y;
	robot->current_position.x = initial_pos.x;

	robot->data = data;

	return robot;
}
void sim_robot_draw(sim_robot_t* robot) {
	
	ALLEGRO_BITMAP *r_bitmap = robot->data->bitmaps->robot;

	double height = al_get_bitmap_width(r_bitmap);
	double width = al_get_bitmap_height(r_bitmap);

	double screen_width  = robot->data->screen_size->x;
	double screen_height = robot->data->screen_size->y;
	double new_size = robot->data->tile_size * ROBOT_FACTOR_SIZE_CONST;

	double sim_width  = robot->data->sim_size->x;
	double sim_height = robot->data->sim_size->y;
	double tile_size = robot->data->tile_size;

	double unit_x = robot->current_position.x;
	double unit_y = robot->current_position.y;
	//printf("%d %d ", screen_width , screen_height);

	double start_x = screen_width / 2.0 - tile_size*sim_width / 2.0;
	double start_y = screen_height / 2.0 - tile_size*sim_height / 2.0;

	double pos_x = start_x + unit_x * tile_size;
	double pos_y = start_y + unit_y * tile_size;

	//cout << pos_x << ' ' << pos_y << '\n';
	if (pos_x - new_size / 2 <= start_x) {
		pos_x = start_x + new_size / 2;
	}
	if (pos_y - new_size / 2 <= start_y) {
		pos_y = start_y + new_size / 2;
	}
	if (pos_x + new_size / 2 >= start_x + sim_width*tile_size) {
		pos_x = start_x + sim_width*tile_size - new_size/2;
	}
	if (pos_y + new_size / 2 >= start_y + sim_height*tile_size) {
		pos_y = start_y+sim_height*tile_size - new_size/2;
	}

	/*** Draw arrow ***/
	uint32_t bitmap_height = al_get_bitmap_height(robot->data->bitmaps->arrow);
	uint32_t bitmap_width  = al_get_bitmap_width(robot->data->bitmaps->arrow);

	al_draw_scaled_rotated_bitmap(robot->data->bitmaps->arrow,
		0, bitmap_height / 2, pos_x, pos_y, tile_size / bitmap_width , tile_size / bitmap_height / 2,
		atan2(robot->target_position.y - robot->start_position.y , robot->target_position.x - robot->start_position.x), 0);

	/*** Draw robot ***/
	al_draw_scaled_bitmap(r_bitmap,
		0, 0, width, height,
		pos_x - new_size / 2, pos_y - new_size / 2,
		new_size, new_size, 0);

}
void sim_robot_update(sim_robot_t* robot) {

	uint32_t current = robot->data->current_it;
	uint32_t period = robot->data->period;

	double tx = robot->target_position.x;
	double ty = robot->target_position.y;

	double x = robot->start_position.x;
	double y = robot->start_position.y;

	//printf("%f %f \n", robot->target_position.x, robot->target_position.y);
	//printf("%d %d \n", current, period);
	robot->current_position.x = x + (tx - x) * (double)current / (double)period;
	robot->current_position.y = y + (ty - y) * (double)current / (double)period;

}
void sim_robot_set_target_position(sim_robot_t * robot, pos_t<double> target) {
	//printf("start = %f %f \n", robot->target_position.x, robot->target_position.y);
	//printf("target = %f %f \n", target.x, target.y);
	robot->start_position = robot->target_position;

	robot->target_position = target;
}
void sim_robot_destroy(sim_robot_t * robot) {
	free(robot);
}


graphic_simluation_t *sim_create(pos_t<uint32_t> *screen_size,pos_t<uint32_t> *sim_size,uint32_t period, uint32_t cnt_robots, pos_t<double> *initial_pos) {
	graphic_simluation_t *sim = (graphic_simluation_t*)malloc(sizeof(graphic_simluation_t));

	sim->robots = (sim_robot_t**)malloc(sizeof(sim_robot_t*)*(int)cnt_robots);
	sim->data.bitmaps = bitmaps_init();
	sim->data.tiles = (bool*)malloc(sizeof(bool)*sim_size->x*sim_size->y);

	if (sim->robots == NULL) return NULL;
	if (sim->data.bitmaps == NULL) return NULL;

	sim->cnt_robots = cnt_robots;
	sim->data.screen_size = screen_size;
	
	sim->data.period = period;
	sim->data.current_it = 0;
	sim->data.sim_size = sim_size;
	sim->cnt_robots = cnt_robots;

	double screen_width = screen_size->x, screen_height = screen_size->y;
	double tile_x_cnt = sim_size->x, tile_y_cnt = sim_size->y;

	sim->data.tile_size = min(screen_width / tile_x_cnt, screen_height / tile_y_cnt);
	sim->data.period = period;
	
	for (int i = 0; i < (int)cnt_robots; i++) {
		sim->robots[i] = sim_robot_init(period,&sim->data,initial_pos[i]);
		if (sim->robots[i] == NULL) return NULL;
	}

	return sim;
}
void sim_update_screen_size(graphic_simluation_t *sim,uint32_t screen_width,uint32_t screen_height) {
	sim->data.screen_size->x = screen_width;
	sim->data.screen_size->y = screen_height;
	sim->data.tile_size = min( (double)screen_width / (double)sim->data.sim_size->x , (double)screen_height / (double)sim->data.sim_size->y );
}
void sim_draw(graphic_simluation_t * sim) {


	ALLEGRO_BITMAP *clean_bitmap = sim->data.bitmaps->clean_tile;
	ALLEGRO_BITMAP *dirty_bitmap = sim->data.bitmaps->dirty_tile;

	double height = al_get_bitmap_width(clean_bitmap);
	double width = al_get_bitmap_height(clean_bitmap);
	double screen_width = sim->data.screen_size->x;
	double screen_height = sim->data.screen_size->y;
	double tile_size = sim->data.tile_size;
	double sim_width = sim->data.sim_size->x;
	double sim_height = sim->data.sim_size->y;

	for (int i = 0; i < (int)sim->data.sim_size->y; i++) {
		for (int j = 0; j < (int)sim->data.sim_size->x; j++) {

			double new_size = sim->data.tile_size;
			double pos_y = screen_height / 2.0 - tile_size*sim_height / 2.0 + (double)i * tile_size + tile_size/2;
			double pos_x = screen_width / 2.0 - tile_size*sim_width / 2.0 + (double)j * tile_size + tile_size/2;
			ALLEGRO_BITMAP *bitmap_use = sim_tile_status(sim,i,j) ? clean_bitmap : dirty_bitmap;
			al_draw_scaled_bitmap(bitmap_use,
				0, 0, width, height,
				pos_x - new_size / 2, pos_y - new_size / 2,
				new_size, new_size, 0);
		}
	}

	for (int i = 0; i < (int)sim->cnt_robots; i++) {
		sim_robot_draw(sim->robots[i]);
	}


}
void sim_update_robot_pos(graphic_simluation_t * sim, pos_t<double> *positions) {
	sim->data.current_it = 0;
	for (int i = 0; i < (int)sim->cnt_robots; i++) {
		sim_robot_set_target_position(sim->robots[i], positions[i]);
	}
}
bool sim_advance_animation(graphic_simluation_t *sim) {

	if (sim->data.current_it < sim->data.period) sim->data.current_it++;

	for (int i = 0; i < (int)sim->cnt_robots; i++) {
		sim_robot_update(sim->robots[i]);
	}

	return sim->data.current_it == sim->data.period;
}
void sim_update_tiles(graphic_simluation_t *sim,bool *tiles) {
	for (int i = 0; i < sim->data.sim_size->y;i++) {
		for (int j = 0; j < sim->data.sim_size->x; j++) {
			sim->data.tiles[i * sim->data.sim_size->x + j] = tiles[i * sim->data.sim_size->x + j];
		}
	}
}
bool sim_tile_status(graphic_simluation_t *sim, uint32_t i, uint32_t j) {
	return sim->data.tiles[i * sim->data.sim_size->x + j];
}
void sim_destroy(graphic_simluation_t * sim) {
	bitmaps_destroy(sim->data.bitmaps);
	free(sim->data.tiles);
	for (int i = 0; i < (int)sim->cnt_robots; i++) {
		sim_robot_destroy(sim->robots[i]);
	}
	free(sim);
	
}

