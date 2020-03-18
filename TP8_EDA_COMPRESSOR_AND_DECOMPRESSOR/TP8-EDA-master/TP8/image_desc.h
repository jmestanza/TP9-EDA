#pragma once
#include <iostream>
#include <allegro5\allegro5.h>

using namespace std;

class image_desc
{
public:
	image_desc();
	void load_path(string path_received); // img path setter
	void load_image();
	bool is_error(); // returns 0 - OK or 1 - ERROR
	bool is_selected(); // returns 0 - NOT or 1 - YES
	const char* get_name();
	string get_stem_name();
	string get_path();
	void select();
	void un_select();
	void toggle_select();
	void draw_img(float x, float y);
	float get_img_draw_w();
	float get_img_draw_h();
	~image_desc();

private:
	bool selected;
	bool error;
	const float w_for_draw = 150; // los mosaicos son del mismo formato de 3x3
	const float h_for_draw = 75;
	float src_w; // informacion para el compresor y el drawer
	float src_h;
	string img_path;
	string img_name; // nombre con la extension
	string img_stem_name; // nombre sin la extension
	ALLEGRO_BITMAP * img_;
};

