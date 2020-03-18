#include "image_desc.h"
#include "filesystem_header.h"

#define UNKNOWN_FILE_IMG_DIR "Question.png"

image_desc::image_desc()
{
	selected = false; // todas empiezan sin seleccionar
	error = false;
	img_ = nullptr;
}

bool image_desc::is_selected()
{
	return selected;
}

const char * image_desc::get_name()
{
	return (img_name.c_str());
}

string image_desc::get_stem_name()
{
	return img_stem_name;
}

string image_desc::get_path()
{
	return img_path;
}

void image_desc::select()
{
	selected = true;
}

void image_desc::un_select()
{
	selected = false;
}

void image_desc::toggle_select()
{
	selected ? (selected = false) : (selected = true);
}

void image_desc::draw_img(float x, float y)
{
	float center_x = x - (w_for_draw / 2);
	float center_y = y - (h_for_draw / 2);
	al_draw_scaled_bitmap(img_, 0, 0, src_w, src_h, center_x, center_y, w_for_draw, h_for_draw, 0);
}

float image_desc::get_img_draw_w()
{
	return w_for_draw;
}

float image_desc::get_img_draw_h()
{
	return h_for_draw;
}


void image_desc::load_path(string path_received)
{
	this->img_path = path_received;
	path aux_name(path_received); 
	this->img_name = aux_name.filename().string(); 
	this->img_stem_name = aux_name.stem().string();
}

void image_desc::load_image()
{
	img_ = al_load_bitmap(UNKNOWN_FILE_IMG_DIR);

	if (img_ == nullptr) 
	{
		error = true;
	}
	else
	{
		src_w = al_get_bitmap_width(img_); // get image original dimensions
		src_h = al_get_bitmap_height(img_);
	}
}

bool image_desc::is_error()
{
	return error;
}

image_desc::~image_desc()
{
}
