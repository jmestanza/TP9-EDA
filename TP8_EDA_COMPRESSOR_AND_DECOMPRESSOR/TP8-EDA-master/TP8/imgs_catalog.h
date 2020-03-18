#pragma once
#include "mosaico.h"
#include <string>
#include <vector>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

using namespace std;

class imgs_catalog
{
public:
	imgs_catalog(vector<string> &imgs_paths, unsigned int th_data);
	unsigned int get_num_imgs();
	bool is_catalog_load_error(); // returns 0 - OK or 1 - ERROR ON LOAD FLECHA
	bool draw_page(unsigned int index); // returns 0 - OK or 1 - ERROR ON INDEX
	void compress_all();
	void toogle_img_state(unsigned int img_num);
	void select_page();
	void un_select_page();
	void next_page();
	void previus_page();
	~imgs_catalog();
private:
	unsigned int tot_imgs;
	unsigned int mosaico_qnt;
	unsigned int current_page;
	bool error;
	vector<mosaico> pages;
	ALLEGRO_BITMAP *arrow_img;
	float arrow_src_w;
	float arrow_src_h;
	const float arr_w_draw = 76; // tamano fijo para mostrar las felchas
	const float arr_h_draw = 72;
};

