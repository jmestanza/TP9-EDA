#include "mosaico.h"
#include "compresor.h"

#define WHITE al_map_rgb(255,255,255)
// Constantes para ubicar las imagenes en el mosaico
// Abarca el caso de mosaico completo de 3x3
const float x_draw_coord[3] = {250, 500, 750};
const float y_draw_coord[3] = {150, 300, 450};

mosaico::mosaico(unsigned int cant_img)
{
	img_qnt = cant_img;
	uncomp_something = false;

	font = al_load_ttf_font("Raleway-Black.ttf", 20, 0);
	(font == nullptr) ? load_error = true : load_error = false;

	checkbox_ch = al_load_bitmap("Blue_checkbox-checked.png");
	(checkbox_ch == nullptr) ? load_error = true : load_error = false;

	checkbox_unch = al_load_bitmap("Blue_checkbox-unchecked.png");
	(checkbox_unch == nullptr) ? load_error = true : load_error = false;

	ch_unch_src_side = al_get_bitmap_width(checkbox_ch);
}

unsigned int mosaico::get_img_qnt(void)
{
	return img_qnt;
}

bool mosaico::load_imgs(vector<string>::iterator it_paths)
{
	bool ret = true;
	for (unsigned int i = 0; i < img_qnt; i++)
	{
		image_desc aux_img; // creacion de objeto imagen en blanco
		imgs_lib.push_back(aux_img);
		imgs_lib[i].load_path(it_paths[i]);
		imgs_lib[i].load_image();
		imgs_lib[i].is_error() ? ret = false : ret = true;
	}
	return ret;
}

bool mosaico::is_load_error()
{
	return load_error;
}

ALLEGRO_FONT * mosaico::get_font()
{
	return font;
}

bool mosaico::is_any_uncomp()
{
	return uncomp_something;
}

void mosaico::draw_mosaico()
{
	for (unsigned int i = 0; i < img_qnt; i++)
	{
		imgs_lib[i].draw_img(x_draw_coord[i % 3], y_draw_coord[i / 3]);
		draw_img_new_state(i);
		al_draw_text(font, WHITE, x_draw_coord[i % 3], y_draw_coord[i / 3] + 40,
			ALLEGRO_ALIGN_CENTER, imgs_lib[i].get_name());
	}
}

bool mosaico::uncompress_imgs()
{
	bool uncomp_error = true;
	string ext = ".png";
	for (unsigned int j = 0; j < img_qnt; j++)
	{
		if (imgs_lib[j].is_selected())
		{
			string out_name = imgs_lib[j].get_stem_name() + ext;
			if (!uncompress_algorithm(imgs_lib[j].get_path(), out_name.c_str())) 
			{
				uncomp_error = false;
			}
			uncomp_something = true;
		}
	}
	return uncomp_error;
}

void mosaico::draw_img_new_state(unsigned int img_index)
{
	if (imgs_lib[img_index].is_selected())
	{
		al_draw_scaled_bitmap(checkbox_ch, 0, 0, ch_unch_src_side, ch_unch_src_side,
			x_draw_coord[img_index % 3] + (imgs_lib[img_index].get_img_draw_w() / 2) + 5,
			y_draw_coord[img_index / 3] - 21,
			ch_unch_src_side, ch_unch_src_side, 0);
	}
	else
	{
		al_draw_scaled_bitmap(checkbox_unch, 0, 0, ch_unch_src_side, ch_unch_src_side,
			x_draw_coord[img_index % 3] + (imgs_lib[img_index].get_img_draw_w() / 2) + 5,
			y_draw_coord[img_index / 3] - 21,
			ch_unch_src_side, ch_unch_src_side, 0);
	}
}

void mosaico::change_img_selection(unsigned int img_num)
{
	imgs_lib[img_num].toggle_select();
}

void mosaico::select_all()
{
	for (unsigned int i = 0; i < img_qnt; i++)
	{
		imgs_lib[i].select();
	}
}

void mosaico::un_select_all()
{
	for(unsigned int i = 0; i < img_qnt; i++)
	{
		imgs_lib[i].un_select();
	}
}

mosaico::~mosaico()
{
	
}
