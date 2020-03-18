// ConsoleApplication2.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "allegro5\allegro5.h"
#include <math.h>
#include <allegro5/allegro_primitives.h>

/* cosas que vienen por linea de comando*/
#define LSTARTO 400
#define LENDO 100
#define XOO 400.0
#define YOO 200.0
#define LEFTANGLEO 60.0
#define RIGHTANGLEO -30.0


#define PI 3.14159265358979323846



typedef struct {
	double x;
	double y;
}pos_t;


typedef	struct {
	pos_t arriba;
	pos_t izq;
	pos_t derecha;
}bordes_t;


void dibujar(pos_t p1, pos_t p2, pos_t p3, double lend, int loop, double *small);

void precomputar_orden(pos_t p1, pos_t p2, pos_t p3, double lend, int loop, double *);


double min(double n1, double n2);
double distance(pos_t p1, pos_t p2);
double lado(pos_t p1, pos_t p2, pos_t p3);
pos_t calculate_new_cm(pos_t p1, pos_t p2, pos_t p3);

void get_three_coords(int lstart, double leftang, double rightang, double xo, double yo, bordes_t*);

void plot_triangle(int lstart, double leftang, double rightang, double xo, double yo, int lend);


int main(void)
{
	// allegro init 
	ALLEGRO_DISPLAY * display = NULL;
	
	if(!al_init()) { //Primera funcion a llamar antes de empezar a usar allegro.
	fprintf(stderr, "failed to initialize allegro!\n");
	return -1;
	}

	if (!al_init_primitives_addon()) {
	fprintf(stderr, "failed to initialize primitives!\n");
	return -1;	
	}

	display = al_create_display(800, 600); // Intenta crear display de 640x480 de fallar devuelve NULL
	if(!display) {
	fprintf(stderr, "failed to create display!\n");
	return -1;
	}

	al_clear_to_color(al_map_rgb(255,255,255)); //Hace clear del backbuffer del diplay al color RGB 255,255,255 (blanco)



	plot_triangle(LSTARTO,LEFTANGLEO,RIGHTANGLEO,XOO,YOO,LENDO);



	al_flip_display(); //Flip del backbuffer, pasa a verse a la pantalla

	al_rest(20.0);
	
	al_destroy_display(display);  
	
	system("pause");
	return 0;

}


void get_three_coords(int lstart, double leftang, double rightang, double xo, double yo, bordes_t *p2aux) {
	
	double l1, l2;

	l2 = lstart / (1 + (tan(leftang * PI / 180.0) / tan((rightang + 90.0) * PI / 180.0)));

	l1 = lstart - l2;

	int y1 = l1* tan(leftang * PI / 180.0);

	p2aux->arriba.x = xo;
	p2aux->arriba.y = yo + (y1* 2.0 / 3.0);
	p2aux->derecha.x = xo + l2;
	p2aux->derecha.y = yo - (y1 / 3.0);
	p2aux->izq.x = xo - l1;
	p2aux->izq.y = yo - (y1 / 3.0);
}

void dibujar(pos_t p1, pos_t p2, pos_t p3, double lend, int loop, double *small) {
	if (small[loop-1]> lend){		
		al_draw_triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, al_map_rgb(255, 0, 0), 1.0);
	
		small[loop] = min(small[loop], lado(p1, p2, p3));
		pos_t p4=calculate_new_cm(p1,p2,p3);
		
		dibujar(p1, p2, p4,lend,loop+1,small);
		dibujar(p2, p3, p4, lend,loop+1,small);
		dibujar(p1, p3, p4, lend,loop+1,small);

	}
}
void precomputar_orden(pos_t p1, pos_t p2, pos_t p3, double lend, int loop, double *small) {
	if (small[loop - 1]> lend) {
		printf("%lf %lf\n", p1.x, p1.y);

		small[loop] = min(small[loop], lado(p1, p2, p3));
		pos_t p4 = calculate_new_cm(p1, p2, p3);
		
		precomputar_orden(p1, p2, p4, lend, loop + 1, small);
		precomputar_orden(p2, p3, p4, lend, loop + 1, small);
		precomputar_orden(p1, p3, p4, lend, loop + 1, small);
	}
}


double min(double n1, double n2) {
	return (n1 > n2) ? n2 : n1;

}
double distance(pos_t p1, pos_t p2) {

	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));

}

double lado(pos_t p1,pos_t p2,pos_t p3){

	return min( distance(p1,p3), min(distance(p1,p2), distance(p2,p3)) ) ;

}	

pos_t calculate_new_cm(pos_t p1, pos_t p2, pos_t p3){
	pos_t p4;
	p4.x = (p1.x + p2.x + p3.x) / 3.0;
	p4.y = (p1.y + p2.y + p3.y) / 3.0;
	return p4;
}


void plot_triangle(int lstart, double leftang, double rightang, double xo, double yo, int lend) {

	double profundidad[20];
	for (int i = 0; i < 19; i++) {
		profundidad[i] = 1e6;
	}
	bordes_t main_borde;
	bordes_t *p2borde = &main_borde;

	get_three_coords(lstart, leftang, rightang, xo, yo, p2borde);

	precomputar_orden(p2borde->arriba, p2borde->derecha, p2borde->izq,lend, 1, profundidad);

	dibujar(p2borde->arriba, p2borde->derecha, p2borde->izq, lend, 1, profundidad);

}