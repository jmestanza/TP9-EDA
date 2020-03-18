#include "stdafx.h"
#include <iostream>
#include <cmath>
#include "worm.h"
using namespace std;


/// Internal
float degcos(float angle);
float degsin(float angle);

float degcos(float angle) {
	return (float)cos(angle * acos(0) * 2.0 / 180.0);
}
float degsin(float angle) {
	return (float)sin(angle * acos(0) * 2.0 / 180.0);
}


Worm::Worm(Physics physics, char * validKeys_, double _x, double _y, int _sentido , int move_stage_period) {
	frame_count = 0;
	this->set_id(0); // worm without id
	this->pos.x = _x;
	this->pos.y = _y;
	this->sentido = _sentido;
	this->state = IDLE;
	this->physics = physics;
	this->error = 0;
	this->move_stage_period = move_stage_period;
	this->validKeys = validKeys_;
}

void Worm::set_position(int _x, int _y) {
	cout << "set position " << _x << ' ' << _y << '\n';
	this->pos.x = _x;
	this->pos.y = _y;
}
void Worm::start_moving(char key_) {

	/*if (sentido != -1 && sentido != 1) {
		this->error = 1; // this makes no sense
		return; 
	}*/
	bool isKeyValid = false;
	if (validKeys[0] == key_)
	{
		if (((this->state) != END_MOVEMENT) && ((this->state) != MOVING) && ((this->state) != JUMPING)) //si se mueve no permite cambiar el sentido
		{
			this->sentido = -1;    //tecla de izquierda.   //-1 es izquierda y 1 es derecha.
			isKeyValid = true;
		}
	}
	if (validKeys[1] == key_)
	{
		if (((this->state) != END_MOVEMENT) && ((this->state) != MOVING) && ((this->state) != JUMPING)) //si se mueve no permite cambiar el sentido
		{
			this->sentido = 1;     //tecla de derecha.
			isKeyValid = true;
		}
	}
	if (isKeyValid)
	{
		if (this->state == IDLE) {
			this->state = MONITOR_MOVING;

			this->frame_count = 0;
		}
		else if (this->state == END_MOVEMENT) {
			this->state = MOVING;
		}
	}
}
void Worm::stop_moving(char key_) {
	if (validKeys[0] == key_ || validKeys[1] == key_)
	{
		if (this->state == MOVING) {

			this->state = END_MOVEMENT;
		}
		else if (this->state == MONITOR_MOVING) {
			this->state = IDLE;
		}
	}
	
}
void Worm::start_jumping(char key_) {
	if (validKeys[2] == key_)
	{
		if (this->state == IDLE) {
			//cout << "jumping called \n";
			this->state = MONITOR_JUMPING;
			this->frame_count = 0;
			this->update_jump_period();
		}
	}
}
void Worm::stop_jumping(char key_) {
	if (validKeys[2] == key_)
	{
		if (this->state == MONITOR_JUMPING) {
			this->state = IDLE;
		}
		else {
			this->error = 1;
		}
	}
	
}

void Worm::update_jump_period() {
	float a = this->physics.jump_angle;
	float v0 = this->physics.jump_speed;
	float t = this->frame_count * this->physics.TIME_PER_UPDATE;
	float g = this->physics.gravity;

	//cout << v0 << ' ' << degsin(a) << '\n';
	float jump_time = 2 * v0 * degsin(a) / (g);

	this->jump_stage_period = (int)ceil(jump_time / this->physics.TIME_PER_UPDATE);
	//cout << jump_time << '\n';
	//cout << jump_time / 0.02 << '\n';
	//cout << this->jump_stage_period << '\n';
}
void Worm::update() {
	float t;

	static int framefounder = 0;

	switch (this->state) {
		case MONITOR_MOVING:
			if (this->frame_count*this->physics.TIME_PER_UPDATE >= 0.1) {
				this->original_pos = this->pos;
				this->state = MOVING;
				this->frame_count = 0;
			}
		break;
		case MONITOR_JUMPING:
			if (this->frame_count*this->physics.TIME_PER_UPDATE >= 0.1) {
				
				this->state = JUMPING;
				this->original_pos = this->pos;
				this->frame_count = 0;
			}
			break;
		case MOVING:
		case END_MOVEMENT:
			t = this->frame_count * this->physics.TIME_PER_UPDATE;

			this->pos.x = this->original_pos.x + (this->sentido * this->physics.speed * t)/2.0;
			this->move_stage = (this->frame_count) % (this->move_stage_period);

			this->correct_range();
			if (((this->state) == END_MOVEMENT) && ((this->frame_count % 50) == 0)) {
				this->state = IDLE;
				this->move_stage = 0;
			}
		break;
	
		case JUMPING:

			float a = this->physics.jump_angle;
			float v0 = this->physics.jump_speed;
			t = this->frame_count * this->physics.TIME_PER_UPDATE;
			
			float g = this->physics.gravity;

			double mruv_y = v0 * degsin(a) * t - g*t*t / 2.0;
			double mruv_x = this->sentido * v0 * degcos(a) * t;
			//cout << "v0 = " << degsin(a) << '\n';

			//cout << mruv_x << '\n';

			this->pos.y = this->original_pos.y - mruv_y;
			this->pos.x = this->original_pos.x + mruv_x; ;

			this->correct_range();
			if (this->frame_count == LAST_JUMPING_FRAME) {
				pos.y = this->original_pos.y;
			}
			

			this->jump_stage = this->frame_count;
			if (this->jump_stage == this->jump_stage_period) {
				this->state = IDLE;
			}
		break;
	}
	(this->frame_count)++;
}
void Worm::correct_range() {
	if (this->pos.x < this->physics.min_coordinates.x) {
		this->pos.x = this->physics.min_coordinates.x;
	}
	if (this->pos.x > this->physics.max_coordinates.x) {
		this->pos.x = this->physics.max_coordinates.x;
	}
	if (this->pos.y < this->physics.min_coordinates.y) {
		this->pos.y = this->physics.min_coordinates.y;
	}
	if (this->pos.y > this->physics.max_coordinates.y) {
		this->pos.y = this->physics.max_coordinates.y;
	}
}
int Worm::get_sentido() { // 1 o -1
	return this->sentido;
}
int Worm::get_state() {
	return this->state;
}
Pos Worm::get_position() {
	return this->pos;
}

Pos  Worm:: get_original_position(){
	return original_pos;
}

int Worm::get_jump_stage_animation() {
	return this->jump_stage;
}
int Worm::get_move_stage_animation() {
	return this->move_stage;
}
int Worm::get_if_error() {
	return this->error;
}

int Worm::get_jump_period()
{
	return jump_stage_period;
}
void Worm::set_keys(char * keys_)
{
	validKeys = keys_;
}

void Worm :: set_physics(Physics physics_)
{
	physics = physics_;
}

void Worm::set_id(int n)
{
	id = n;
}

void Worm::set_sentido(bool a)
{
	sentido = a;
}

int Worm::get_id(void)
{
	return id;
}
