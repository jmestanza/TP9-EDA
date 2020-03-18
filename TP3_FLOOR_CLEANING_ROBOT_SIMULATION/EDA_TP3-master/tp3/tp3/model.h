#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>
#include "utils.h"

// auxiliar robot functions
static bool valid_move(pos_t<double> *pos, double angle);
static void move(pos_t<double> *pos, double angle);

/*** robot_t ***/
struct robot_t {
	pos_t<double> pos;
	pos_t<uint32_t> *model_size;
	double angle;
};

robot_t *robot_init(pos_t<double> ,double,pos_t<uint32_t> *);
void robot_move(robot_t *robot);
pos_t<double> robot_get_pos(robot_t* robot);
void robot_destroy(robot_t *robot);
/*** end robot_t ***/

/*** start piso_t ***/
struct piso_t {
	bool *baldosas;
	pos_t<uint32_t> *model_size;
	uint32_t limpias;
};

piso_t *piso_init(pos_t<uint32_t> *msize);
void piso_set_baldosa(piso_t*,uint32_t i, uint32_t j, uint32_t value);
bool piso_get_baldosa(piso_t*,uint32_t i, uint32_t j);
void piso_destroy(piso_t*);

/*** end_piso_t ***/

/*** start model_t ***/
struct model_t {
	robot_t** robots;
	uint32_t robot_cnt;
	pos_t<uint32_t> model_size;
	uint64_t tick_count;
	piso_t *piso;
	uint32_t configured_robots;
};

model_t * model_init(uint32_t robot_cnt, uint32_t width, uint32_t height);
void model_update_cycle(model_t *);
void model_full_simulation(model_t *);
void model_show_robots(model_t *model);
pos_t<double> * model_get_all_positions(model_t *model);
void model_destroy(model_t *);
void model_reset(model_t* model, uint32_t active_robots);
bool model_end(model_t *model);

/*** end model_t ***/

#endif //model_h