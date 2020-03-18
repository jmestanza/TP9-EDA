#pragma once
#include "image_desc.h"
#include <vector>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

class mosaico
{
public:
	mosaico(unsigned int images_cant);
	unsigned int get_img_qnt();
	bool load_imgs(vector<string>::iterator it_paths); // returns 1 - OK or 0 - ERROR ON LOAD
	bool is_load_error();
	ALLEGRO_FONT* get_font();
	bool is_any_uncomp();
	void draw_mosaico();
	bool uncompress_imgs(); // returns 1 - OK or 0 - ERROR ON COMPRESSION
	void draw_img_new_state(unsigned int img_index);
	void change_img_selection(unsigned int img_num);
	void select_all();
	void un_select_all();
	~mosaico();

private:
	unsigned int img_qnt; // el maximo es de 9 imagenes por mosaico
	bool load_error;
	bool uncomp_something;
	ALLEGRO_FONT *font; // se asumen inicializados los addons de font y ttf
	ALLEGRO_BITMAP *checkbox_ch;
	float ch_unch_src_side;
	ALLEGRO_BITMAP *checkbox_unch;
	vector<image_desc> imgs_lib;
};

