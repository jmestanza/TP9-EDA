#include "imgs_catalog.h"
#include <boost/lexical_cast.hpp>

#define MOSAIC_SIZE 9
#define WHITE al_map_rgb(255,255,255)
#define BLACK al_map_rgb(0, 0, 0)

const float left_arr_x = 375;
const float left_arr_y = 525;
const float right_arr_x = 625;
const float right_arr_y = 525;

const float page_compTitle_x = 500;
const float page_compTitle_y = 20;
const float page_numText_x = 500;
const float page_numText_y = 545;

imgs_catalog::imgs_catalog(vector<string> &imgs_paths)
{
	arrow_img = al_load_bitmap("flecha.png");
	if (arrow_img != nullptr)
	{
		error = false;
		current_page = 0;
		arrow_src_w = al_get_bitmap_width(arrow_img);
		arrow_src_h = al_get_bitmap_height(arrow_img);
		tot_imgs = (unsigned int)(imgs_paths.size());
		vector<string>::iterator it = imgs_paths.begin(); // para recorrer todo el vector
		mosaico_qnt = tot_imgs / 9; // cantidad de mosaicos completos

		unsigned int j = 0;
		for (j = 0; j < mosaico_qnt; j++) // creo los mosaicos completos
		{
			mosaico aux_m(MOSAIC_SIZE);
			pages.push_back(aux_m);
			pages[j].load_imgs(it + (MOSAIC_SIZE*j));
		}

		unsigned int sobrante = tot_imgs % 9;
		sobrante ? mosaico_qnt++ : false;

		mosaico aux_m(sobrante);
		pages.push_back(aux_m); // creo el mosaico de las sobrantes
		pages[j].load_imgs(it + (MOSAIC_SIZE*j));
	}
	else
	{
		error = true;
	}
}

unsigned int imgs_catalog::get_num_imgs()
{
	return tot_imgs;
}

bool imgs_catalog::is_catalog_load_error()
{
	return error;
}

bool imgs_catalog::draw_page(unsigned int index)
{
	if (index < mosaico_qnt)
	{
		float R_x = (right_arr_x + 10 - (arr_w_draw / 2));
		float R_y = (right_arr_y + 30 - (arr_w_draw / 2));
		float L_x = (left_arr_x - 10 - (arr_w_draw / 2));
		float L_y = (left_arr_y + 30 - (arr_h_draw / 2));

		pages[index].draw_mosaico();

		al_draw_scaled_bitmap(arrow_img, 0, 0, arrow_src_w, arrow_src_h, 
			L_x, L_y, arr_w_draw, arr_h_draw, ALLEGRO_FLIP_HORIZONTAL);
		al_draw_scaled_bitmap(arrow_img, 0, 0, arrow_src_w, arrow_src_h,
			R_x, R_y, arr_w_draw, arr_h_draw, 0);

		string current_page = "Page " + boost::lexical_cast<std::string>(index+1) + " de " + boost::lexical_cast<std::string>(mosaico_qnt);
		al_draw_text(pages[index].get_font(), WHITE, page_compTitle_x, page_compTitle_y,
			ALLEGRO_ALIGN_CENTER, "IMAGE UNCOMPRESSOR");
		al_draw_text(pages[index].get_font(), WHITE, page_compTitle_x, page_compTitle_y + 21,
			ALLEGRO_ALIGN_CENTER, "Select images to uncompress with 1 - 9");
		al_draw_text(pages[index].get_font(), WHITE, page_compTitle_x, page_compTitle_y + 21 + 21,
			ALLEGRO_ALIGN_CENTER, "Press arrows to move through pages");
		al_draw_text(pages[index].get_font(), WHITE, page_numText_x, page_numText_y, 
			ALLEGRO_ALIGN_CENTER, current_page.c_str());
		
		al_flip_display();
		return false;
	}
	else
	{
		return true;
	}
}

void imgs_catalog::uncompress_all()
{
	al_clear_to_color(BLACK);
	al_draw_text(pages[0].get_font(), WHITE, 1000 / 2, 600 / 2, ALLEGRO_ALIGN_CENTER,
		"Uncompressing...");
	al_flip_display();

	bool process_error = false;
	for (unsigned int i = 0; i < mosaico_qnt; i++)
	{
		if (!pages[i].uncompress_imgs())
		{
			process_error = true;
		}
	}

	if (!process_error)
	{
		bool uncomp_flag = false;
		for (unsigned int i = 0; i < mosaico_qnt; i++)
		{
			for (unsigned int j = 0; j < pages[i].get_img_qnt(); j++)
			{
				pages[i].is_any_uncomp() ? uncomp_flag = true : false;
			}
		}

		if (uncomp_flag)
		{
			al_clear_to_color(BLACK);
			al_draw_text(pages[0].get_font(), WHITE, 1000 / 2, 600 / 2, ALLEGRO_ALIGN_CENTER,
				"Uncompressing complete. Close display to exit.-");
			al_flip_display();
		}
		else
		{
			al_clear_to_color(BLACK);
			al_draw_text(pages[0].get_font(), WHITE, 1000 / 2, 600 / 2, ALLEGRO_ALIGN_CENTER,
				"Nothing to uncompress selected. Close display to exit.-");
			al_flip_display();
		}
	}
	else
	{
		al_clear_to_color(BLACK);
		al_draw_text(pages[0].get_font(), WHITE, 1000 / 2, 600 / 2, ALLEGRO_ALIGN_CENTER,
			"Some files were not uncompressed. See CMD for info. Close display to exit.-");
		al_flip_display();
	}
}

void imgs_catalog::toogle_img_state(unsigned int img_num)
{
	if ((img_num > 0) && ((img_num-1) < (pages[current_page].get_img_qnt()))) 
	{
		pages[current_page].change_img_selection(img_num-1);
		al_clear_to_color(BLACK);
		draw_page(current_page);
	}
}

void imgs_catalog::select_page()
{
	pages[current_page].select_all();
	al_clear_to_color(BLACK);
	draw_page(current_page);
}

void imgs_catalog::un_select_page()
{
	pages[current_page].un_select_all();
	al_clear_to_color(BLACK);
	draw_page(current_page);
}

void imgs_catalog::next_page()
{
	if (current_page < mosaico_qnt-1)
	{
		current_page++;
		al_clear_to_color(BLACK);
		draw_page(current_page);
	}
}

void imgs_catalog::previus_page()
{
	if (current_page > 0)
	{
		current_page--;
		al_clear_to_color(BLACK);
		draw_page(current_page);
	}
}


imgs_catalog::~imgs_catalog()
{
}
