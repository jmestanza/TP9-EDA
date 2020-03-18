#include "stdafx.h"
#include "output.h"

#define IDLE_FRAME 3
#define WARM_UP 3
#define WALK_CYCLE 13
#define UNWIND_FRAMES 7
bool InitializeAllegroOutput(void);
//Inicializa los  addons necesarios de allegro para
//utilizar el modulo de output.



void destruir_canciones(ALLEGRO_SAMPLE**, int);
//Funcion que libera el espacio de todas la canciones cargadas.
//recibe un puntero al arreglo de dichas canciones y el numero de las mismas.



ALLEGRO_BITMAP * load_image_at_size(char* image_name, int size_x, int size_y);
//Devuelve el bitmap de la imagen cargada en el tamano deseado.
//En caso de error devuelve NULL.

void destroy_images(ALLEGRO_BITMAP  **imagen, unsigned int num_imagenes);
//Libera toda la memoria utilizada por las imagenes creadas.


viewer::viewer(unsigned int width_, unsigned int height_, unsigned int n_worms)
{
	height = height_;
	width = width_;
	graph_pos = new Pos[n_worms];
	char* jump_path[J_FRAMES] = { J_F0, J_F1, J_F2, J_F3, J_F4, J_F5, J_F6, J_F7, J_F8, J_F9 }; //Paths de las imagenes de jump
	char* walk_path[W_FRAMES] = { W_F0, W_F1, W_F2, W_F3, W_F4, W_F5, W_F6, W_F7, W_F8, W_F9, W_F10, W_F11, W_F12, W_F13, W_F14 }; //paths de las imagenes de walk

	init = InitializeAllegroOutput();
	if (init)
	{
		init = InitializeResources(jump_path, walk_path);
	}

}

viewer:: ~viewer()
{
	if (init)
	{
		delete[] graph_pos;
		
		al_stop_samples();
		al_destroy_bitmap(background);
		al_destroy_bitmap(landscape);
		
		destroy_images(worm_jump, J_FRAMES);
		destroy_images(worm_walk, W_FRAMES);
		destruir_canciones(&music, NUMBER_OF_SAMPLES);
		al_destroy_display(display);

	}
}

bool viewer::IsInitOK(void)
{
	return init;
}

void viewer::UpdateDisplay(Worm* worms, unsigned int worm_count)
{
	ALLEGRO_BITMAP* current_target = al_get_target_bitmap(); //guarda el target actual para no perderlo.

	al_set_target_backbuffer(display);
	al_clear_to_color(al_color_name("black"));
	al_draw_bitmap(landscape, 0, 0, 0);
	al_draw_bitmap(background, 0, 0, 0);

	//al_flip_display();
	int state = 0;
	int facing = 0; //Sentido en el que mira el gusano.
	int jump_stage = 0;
	int walk_stage = 0;
	int period = 0;
	int secuence = 0; //indica en que frame de la accion se encuentra el worm
	for (unsigned int i = 0; i < worm_count; i++)
	{
			state = ((worms + i)->get_state());
			facing = ((worms + i)->get_sentido());
			switch (state)
			{
			case MOVING:
			case END_MOVEMENT:
				walk_stage = (worms + i)->get_move_stage_animation();
				walk_stage %= 50;

				if (walk_stage == 1)
				{
					(graph_pos[i]).x = ((worms + i)->get_position()).x;
					(graph_pos[i]).y = ((worms + i)->get_position()).y;
				}

				if (walk_stage <= 0)
				{
					walk_stage = 1;
					(graph_pos[i]).x = ((worms + i)->get_position()).x;
					(graph_pos[i]).y = ((worms + i)->get_position()).y;
				}
				if (walk_stage <= WARM_UP)
				{
					PrintMove(worms[i], walk_stage - 1, facing, i);
				}
				else
				{
					walk_stage -= WARM_UP;
					if (((walk_stage % WALK_CYCLE) == 0))
					{
						if (facing == RIGHT)
						{
							(graph_pos[i]).x += 3.4;
						}
						else
						{
							(graph_pos[i]).x -= 3.4;
						}

						PrintMove(worms[i], IDLE_FRAME, facing, i);
					}
					else
					{
						PrintMove(worms[i], IDLE_FRAME + (walk_stage%WALK_CYCLE) - 1, facing, i);
					}
				}
				break;

			case JUMPING:
				jump_stage = (worms + i)->get_jump_stage_animation();
				period = (worms + i)->get_jump_period();


				if (jump_stage <= 0)
				{
					jump_stage = 1;
				}
				if (jump_stage <= WARM_UP)
				{

					PrintJump(worms[i], jump_stage - 1, facing);
				}

				jump_stage %= period;

				if ((jump_stage >= (period - 6))) //unwind del salto
				{
					PrintJump(worms[i], IDLE_FRAME + UNWIND_FRAMES - (period - jump_stage), facing);
				}
				else
				{
					PrintJump(worms[i], IDLE_FRAME, facing);
				}
				break;

			case MONITOR_MOVING:
			case MONITOR_JUMPING:
			case IDLE:
			default: //En caso de recibir algun state desconocido lo trata com si fuera IDLE.
				PrintPos(worms[i], facing);
				break;

			}
	}
	al_set_target_bitmap(current_target);
}


bool InitializeAllegroOutput(void)
{
	if (!al_init_image_addon())
	{
		return false;
	}
	if (!al_install_audio())
	{
		return false;
	}
	if (!al_init_acodec_addon())
	{
		return false;
	}
	if (!al_reserve_samples(NUMBER_OF_SAMPLES))
	{
		return false;
	}

	return true;
}
ALLEGRO_DISPLAY* viewer::GetDisplay(void)
{
	return display;
}

bool viewer::InitializeResources(char** worm_jumps, char** worm_walks)
{
	
		if (!LoadSong(MUSIC_PATH, NUMBER_OF_SAMPLES))
		{
			return false;
		}

	display = al_create_display(width*(UNIT), height*(UNIT));
	//al_play_sample(music, VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	if (display == NULL)
	{
		al_stop_samples();
		destruir_canciones(&music, NUMBER_OF_SAMPLES);
		return false;
	}


	background = load_image_at_size(BACKGROUND_PATH, width*(UNIT), height*(UNIT)); //crea el bitmap con el background
	if (background == nullptr)
	{
		al_stop_samples();
		destruir_canciones(&music, NUMBER_OF_SAMPLES);
		al_destroy_display(display);
		return false;
	}

	landscape = load_image_at_size(LANDSCAPE_PATH, width*(UNIT), height*(UNIT));
	if (landscape == nullptr)
	{
		al_stop_samples();
		destruir_canciones(&music, NUMBER_OF_SAMPLES);
		al_destroy_display(display);
		al_destroy_bitmap(background);
		return false;
	}
	for (unsigned int i = 0; i < W_FRAMES; i++) //Crea los bitmaps con los frames del worm walk.
	{
		worm_walk[i] = load_image_at_size(worm_walks[i], WORM_SIZE, WORM_SIZE);
		if ((worm_walk[i]) == nullptr)
		{
			al_stop_samples();
			destruir_canciones(&music, NUMBER_OF_SAMPLES);
			al_destroy_display(display);
			al_destroy_bitmap(background);
			al_destroy_bitmap(landscape);
			destroy_images(worm_walk, i);
			return false;
		}
	}

	for (unsigned int i = 0; i < J_FRAMES; i++) //Crea los bitmaps con los frames del worm jump.
	{
		worm_jump[i] = load_image_at_size(worm_jumps[i], WORM_SIZE, WORM_SIZE);
		if ((worm_jump[i]) == nullptr)
		{
			al_stop_samples();
			destruir_canciones(&music, NUMBER_OF_SAMPLES);
			al_destroy_display(display);
			al_destroy_bitmap(background);
			al_destroy_bitmap(landscape);
			destroy_images(worm_walk, W_FRAMES);
			destroy_images(worm_jump, i);
			return false;
		}
	}

	return true;

}

void destroy_images(ALLEGRO_BITMAP  **imagen, unsigned int num_imagenes)
{
	unsigned int i;
	for (i = 0; i<num_imagenes; i++)
	{
		al_destroy_bitmap(imagen[i]);
	}
}

ALLEGRO_BITMAP* load_image_at_size(char* image_name, int size_x, int size_y)
{
	ALLEGRO_BITMAP * image = nullptr;
	ALLEGRO_BITMAP * current_target = nullptr;
	ALLEGRO_BITMAP * resized_image = nullptr;

	current_target = al_get_target_bitmap(); //salva el target original para no perderlo

	image = al_load_bitmap(image_name);
	if (image == NULL)
	{
		return nullptr;
	}

	resized_image = al_create_bitmap(size_x, size_y);
	if (resized_image == NULL)
	{
		al_destroy_bitmap(image);
		return nullptr;


	}

	al_set_target_bitmap(resized_image);
	al_draw_scaled_bitmap(image, 0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
		0, 0, size_x, size_y, 0);

	al_set_target_bitmap(current_target); //vuelve al target original
	al_destroy_bitmap(image);
	return resized_image;
}
bool viewer:: LoadSong( char * nombre, int numero_de_canciones)
{
	int i;
	int valido = true;
	for (i = 0; (i<numero_de_canciones) && (valido); i++)
	{
		music = al_load_sample(nombre);
		if (music == NULL)
		{
			valido = false;
		}

	}

	return valido;



}

void destruir_canciones(ALLEGRO_SAMPLE** canciones, int numero_de_canciones)
{
	int i;
	for (i = 0; i< numero_de_canciones; i++)
	{
		al_destroy_sample(canciones[i]);
	}

}

void viewer::PrintMove(Worm& worm, int secuence_, int sense, unsigned int n_worm)
{
	double wormX = (graph_pos[n_worm]).x;
	double wormY = (graph_pos[n_worm]).y;
	int secuence = secuence_ % W_FRAMES;
	al_set_target_backbuffer(display);
	if (sense == RIGHT)
	{
		al_draw_bitmap(worm_walk[secuence], wormX, wormY, ALLEGRO_FLIP_HORIZONTAL);
	}
	else
	{
		al_draw_bitmap(worm_walk[secuence], wormX, wormY, 0);
	}
}

void viewer::PrintJump(Worm& worm, int secuence_, int sense)
{
	al_set_target_backbuffer(display);
	double wormX = ((worm.get_position()).x);
	double wormY = ((worm.get_position()).y);
	int secuence = secuence_ % J_FRAMES;
	if (sense == RIGHT)
	{
		al_draw_bitmap(worm_jump[secuence], wormX, wormY, ALLEGRO_FLIP_HORIZONTAL);
	}
	else
	{
		al_draw_bitmap(worm_jump[secuence], wormX, wormY, 0);
	}
}

void viewer::PrintPos(Worm& worm, int sense)
{
	double wormX = (worm.get_position()).x;
	double wormY = (worm.get_position()).y;
	al_set_target_backbuffer(display);
	if (sense == RIGHT)
	{
		al_draw_bitmap(worm_walk[IDLE_FRAME], wormX, wormY, ALLEGRO_FLIP_HORIZONTAL);
	}
	else
	{
		al_draw_bitmap(worm_walk[IDLE_FRAME], wormX, wormY, 0);
	}
}
void viewer::flipViewer(void)
{
	al_flip_display();
}