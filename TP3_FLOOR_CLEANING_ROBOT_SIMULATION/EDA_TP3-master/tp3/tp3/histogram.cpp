#include "histogram.h"

int printHistogram(double* result, pos_t<double>* p2scr, uint32_t cant) {
	ALLEGRO_DISPLAY *display = NULL;
	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	display = al_create_display(p2scr->x, p2scr->y);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}
	if (!al_init_primitives_addon()) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}
	
	if (!al_init_font_addon()) {
		fprintf(stderr, "failed to create font!\n");
		return -1;
	}
	al_init_ttf_addon();

	ALLEGRO_FONT * font1 = al_load_font("arial.ttf", 12, 0);

	double partition_x = p2scr->x / (double(cant) + 1.0);

	double epsylon = partition_x / 4.0;
	double BASEY = 100.0; double TOPY = 50;
	double offset_ticks_text = 25.0;
	double offset_numbers_text = 75.0;
	double partition_y = ((p2scr->y - BASEY - TOPY) / (result[0] + 2.0));
	
	

	for (int i = 1; i < cant + 1; i++) {
		// itero el array obteniendo los valores y graphicando una vez q hago todo updateo el display	
		al_draw_rectangle(i*partition_x + epsylon, p2scr->y - BASEY, i*partition_x - epsylon, p2scr->y - (result[i - 1] * partition_y)-BASEY, al_map_rgb(255, 0, 0), 0.0);
		std::string s = std::to_string(i);
		const char* p2str = &s[0];
		al_draw_text(font1, al_map_rgb(0, 255, 0), i*partition_x, p2scr->y - BASEY + TOPY, 0, p2str);
	}

	al_draw_text(font1, al_map_rgb(255, 255, 255), partition_x, offset_ticks_text, 0, "TICKS");

	al_draw_text(font1, al_map_rgb(255, 255, 255), (cant-5)*partition_x, p2scr->y - BASEY + offset_numbers_text, 0, "CANT ROBOTS");

	al_flip_display();

	return 0;
}