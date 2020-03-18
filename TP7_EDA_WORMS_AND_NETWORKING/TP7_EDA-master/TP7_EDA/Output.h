
#ifndef OUTPUT_H
#define OUTPUT_H
//Este modulo asume que ya se incializo exitosamente Allegro.

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_color.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include "worm.h"

#define WORM_SIZE 40
#define UNIT 1
#define J_FRAMES 10
#define W_FRAMES 15
//Constantes de Audio
#define NUMBER_OF_SAMPLES 1
#define MUSIC_PATH "Music/Get_Ready_For_This_8-bit_.flac"
#define VOLUME 1

//Imagen de fondo
#define BACKGROUND_PATH "Images/Scenario.png"
#define LANDSCAPE_PATH "Images/AshLake.png"

//Imagenes del worm walk
#define W_F0 "Images/wwalking/wwalk-F1.png"
#define W_F1 "Images/wwalking/wwalk-F2.png"
#define W_F2 "Images/wwalking/wwalk-F3.png"
#define W_F3 "Images/wwalking/wwalk-F4.png"
#define W_F4 "Images/wwalking/wwalk-F5.png"
#define W_F5 "Images/wwalking/wwalk-F6.png"
#define W_F6 "Images/wwalking/wwalk-F7.png"
#define W_F7 "Images/wwalking/wwalk-F8.png"
#define W_F8 "Images/wwalking/wwalk-F9.png"
#define W_F9 "Images/wwalking/wwalk-F10.png"
#define W_F10 "Images/wwalking/wwalk-F11.png"
#define W_F11 "Images/wwalking/wwalk-F12.png"
#define W_F12 "Images/wwalking/wwalk-F13.png"
#define W_F13 "Images/wwalking/wwalk-F14.png"
#define W_F14 "Images/wwalking/wwalk-F15.png"

//Imagenes del worm jump

#define J_F0 "Images/wjump/wjump-F1.png"
#define J_F1 "Images/wjump/wjump-F2.png"
#define J_F2 "Images/wjump/wjump-F3.png"
#define J_F3 "Images/wjump/wjump-F4.png"
#define J_F4 "Images/wjump/wjump-F5.png"
#define J_F5 "Images/wjump/wjump-F6.png"
#define J_F6 "Images/wjump/wjump-F7.png"
#define J_F7 "Images/wjump/wjump-F8.png"
#define J_F8 "Images/wjump/wjump-F9.png"
#define J_F9 "Images/wjump/wjump-F10.png"


class viewer //Se ocupa de todo lo relacionado con el output de la simulacion
{
public:
	viewer(unsigned int width_, unsigned int height_, unsigned int n_worms);
	~viewer();
	void UpdateDisplay(Worm* worms, unsigned int worm_count); //No llama a al_flip_display()
	bool IsInitOK(void);				//deveulve true si todos los recursos se crearon exitosamente.
	ALLEGRO_DISPLAY* GetDisplay(void);
	void flipViewer(void); //hace flip display.

private:

	unsigned int height;
	unsigned int width;
	Pos* graph_pos;
	ALLEGRO_SAMPLE* music;
	ALLEGRO_DISPLAY* display;
	ALLEGRO_BITMAP* background;
	ALLEGRO_BITMAP* landscape;
	ALLEGRO_BITMAP* worm_jump[J_FRAMES]; //guarda los bitmaps con las imagenes correspondientes al salto
	ALLEGRO_BITMAP* worm_walk[W_FRAMES]; //guarda los bitmaps con las imagenes correspondientes al desplazamiento en tierra.
	bool init; //indica si hubo o no error en la inicializacion.
	bool InitializeResources(char** worm_jumps, char** worm_walks);
	bool LoadSong(char*, int);
	void PrintMove(Worm& worm, int secuence, int sense, unsigned int n_worm); //Actualiza la representacion del worm moviendose.
	void PrintJump(Worm& worm, int secuence, int sense); //Actualiza la representacion del worm saltando.
	void PrintPos(Worm& worm, int sense);

};

#endif //OUTPUT_H