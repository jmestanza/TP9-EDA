#ifndef WORM_H
#define WORM_H

#define RIGHT 1
#define LEFT -1

enum { IDLE, MONITOR_MOVING, MOVING, END_MOVEMENT, JUMPING, MONITOR_JUMPING };

#define LAST_JUMPING_FRAME 42

//// We as

struct Pos {
	double x, y;
};

struct Physics {
	float speed; // speed
	float jump_speed;
	float jump_angle;
	float gravity;
	Pos max_coordinates , min_coordinates;

	float TIME_PER_UPDATE;
};

class Worm {
	private:
	//ESTO SERIA LO PRIVATE, PERO LO HAGO PUBLIC PARA NO HACER LOS SETTERS
		int id;

		Pos pos , original_pos;
		int sentido;
		int frame_count;
		int state;
		int error;
		Physics physics;

		int move_stage;
		int jump_stage;

		int jump_stage_period;
		int move_stage_period;

		void correct_range();
		void update_jump_period();

		char * validKeys;
		/*la idea de los char keys es que validKeys apunte a un arreglo, en principio de 3 chars
		que representen las teclas, el primer elemento es la tecla de mover izquierda, el segundo elemento es
		la tecla de mover derecha y la tercer tecla es la de saltar. la idea es que cada vez que el worm comienze
		o deje de moverse, antes revise que la tecla realmente haya sido para el. obviamente hay que buscar una mejor manera
		para que la clase incluya estas letras, ya que desde afuera podria hacerse apuntar a un arreglo de menos elementos
		y tener problemas al desreferenciar. una solucion posible es VECTOR. ahora lo hice asi para que se vea la idea
		y se entienda sin complicarmela con vector.*/

		//ESTO SERIA LO PUBLIC
public:
	Worm(Physics physics = { 0 }, char * validKeys_ = NULL, double _x = 850, double _y = 616, int _sentido = RIGHT, int move_stage_period = 50);

		void start_moving(char key_);
		void stop_moving(char key_);
		void start_jumping(char key_);
		void stop_jumping(char key_);
		void update();

		int get_sentido(); // 1 o -1 (1 es a la derecha, -1 es a la izquierda
		int get_state();
		Pos get_position();
		Pos get_original_position();
		int get_jump_stage_animation();
		int get_move_stage_animation();
		int get_jump_period();
		int get_if_error();
		void set_keys(char * keys_);
		void set_physics(Physics physics_);
		void set_id(int);
		void set_sentido(bool);
		void set_position(int, int);
		int get_id(void);
};

#endif //WORM_H