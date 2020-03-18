#include "allegro_init.h"
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

int allegro_init(void) {
	if (!al_init()) {
		return -1;
	}
	if (!al_install_keyboard())
	{
		return -1;
	}

	if (!al_init_image_addon()) {
		return -1;
	}

	al_init_font_addon(); 
	al_init_ttf_addon();

	return 0;
}

