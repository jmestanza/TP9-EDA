#include "stdafx.h"
#include "allegro5\allegro5.h"
#include <math.h>
#include <allegro5/allegro_primitives.h>

/* cosas que vienen por linea de comando*/
#define TRUE 1
#define FALSE 0

#define LSTARTO 400
#define LENDO 50
#define XOO 400.0
#define YOO 300.0
#define LEFTANGLEO 70.0
#define RIGHTANGLEO -70.0

#define MAX_HEIGHT 600

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

typedef struct {
	int lstart;
	int lend;
	double xo;
	double yo;
	double leftang;
	double rightang;
}trivars_t;

void dibujar(pos_t p1, pos_t p2, pos_t p3, double lend, int loop, double *small);

void precomputar_orden(pos_t p1, pos_t p2, pos_t p3, double lend, int loop, double *);


double min(double n1, double n2);
double distance(pos_t p1, pos_t p2);
double lado(pos_t p1, pos_t p2, pos_t p3);
pos_t calculate_new_cm(pos_t p1, pos_t p2, pos_t p3);

void get_three_coords(trivars_t * p2tri, bordes_t*);
void plot_triangle(trivars_t * p2tri);
int triag_callback( trivars_t *p2trivar);


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

	display = al_create_display(1200, MAX_HEIGHT); // Intenta crear display de 640x480 de fallar devuelve NULL
	if(!display) {
	fprintf(stderr, "failed to create display!\n");
	return -1;
	}

	al_clear_to_color(al_map_rgb(255,255,255)); //Hace clear del backbuffer del diplay al color RGB 255,255,255 (blanco)

	trivars_t triag = { LSTARTO,LENDO,XOO,YOO,LEFTANGLEO,RIGHTANGLEO };
	trivars_t *p2triag = &triag;


	printf("%d", triag_callback(p2triag));

	plot_triangle(p2triag);

	al_flip_display(); //Flip del backbuffer, pasa a verse a la pantalla

	al_rest(20.0);
	
	al_destroy_display(display);  
	
	system("pause");
	return 0;

}

int triag_callback( trivars_t *p2trivar) {
	int noerror = FALSE;
	if (p2trivar->xo < 500.0 && p2trivar->xo > 200.0) {
		noerror = TRUE;
	}else {
		noerror = FALSE;
	}

	if (p2trivar->yo < 500.0 && p2trivar->yo  > 200.0) {
		noerror = TRUE;
	}
	else {
		noerror = FALSE;
	}

	if (p2trivar->lstart > 0 && p2trivar->lstart <= 500) {
		noerror = TRUE;
	}else {
		noerror = FALSE;
	}

	if (p2trivar->lend > (p2trivar->lstart / 10.0) && p2trivar->lend < p2trivar->lstart) {
		noerror = TRUE;
	}
	else {
		noerror = FALSE;

	}
	
	if (p2trivar->rightang < 0.0 && p2trivar->rightang  > -90.0) {
		noerror=TRUE;
	}else {
		noerror = FALSE;
	}

	if (p2trivar->leftang > 0.0 && p2trivar->leftang < 90.0) {
		noerror = TRUE;

	}else {
		noerror = FALSE;
	}

	return noerror;
}

void get_three_coords( trivars_t * p2triag, bordes_t *p2aux) {
	/* se tiene que definir las primeras tres coordenadas para poder ejecutar la funcion recursiva
	y se realizo mediante relaciones trigonometricas*/
	
	double l1, l2;

	l2 = p2triag->lstart / (1 + (tan( (p2triag->rightang+ 90.0 )* PI / 180.0) / tan((p2triag->leftang) * PI / 180.0)));

	l1 = p2triag->lstart - l2;

	int y1 = l1* tan(p2triag->leftang * PI / 180.0);

	p2aux->arriba.x = p2triag->xo;

	p2aux->arriba.y = MAX_HEIGHT -( p2triag->yo + (y1* 2.0 / 3.0));

	p2aux->derecha.x = p2triag->xo + l2;

	p2aux->derecha.y = MAX_HEIGHT-( p2triag->yo - (y1 / 3.0));
	
	p2aux->izq.x = p2triag->xo - l1;


	p2aux->izq.y = MAX_HEIGHT- (p2triag->yo - (y1 / 3.0));
	
}

void dibujar(pos_t p1, pos_t p2, pos_t p3, double lend, int loop, double *small) {
	// podemos pensar el triangulo como un grafo 3-ario
	// donde cada triangulo "padre" tiene 3 "hijos" por cada nivel cada hijo se vuelve padre
	// y cada uno de esos nuevos padres vuelve a tener 3 hijos cada uno

	if (small[loop-1]> lend){		
		al_draw_triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, al_map_rgb(255, 0, 0), 1.0);
		// primero dibujamos al padre
		small[loop] = min(small[loop], lado(p1, p2, p3));
		pos_t p4=calculate_new_cm(p1,p2,p3);
		
		// y despues sus hijos
		dibujar(p1, p2, p4,lend,loop+1,small);
		dibujar(p2, p3, p4, lend,loop+1,small);
		dibujar(p1, p3, p4, lend,loop+1,small);

	}
}
void precomputar_orden(pos_t p1, pos_t p2, pos_t p3, double lend, int loop, double *small) {
	if (small[loop - 1]> lend) {
		
		small[loop] = min(small[loop], lado(p1, p2, p3));
		// tenemos dos condiciones de corte 
		// o bien por profundidad del grafo o por que alguno de los lados supero a lend

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


void plot_triangle(trivars_t * p2tri) {

	double profundidad[20];
	for (int i = 0; i < 19; i++) {
		profundidad[i] = 1e6;
	}

	bordes_t main_borde;
	bordes_t *p2borde = &main_borde;

	get_three_coords(p2tri, p2borde);

	precomputar_orden(p2borde->arriba, p2borde->derecha, p2borde->izq, p2tri->lend, 1, profundidad);

	dibujar(p2borde->arriba, p2borde->derecha, p2borde->izq, p2tri->lend, 1, profundidad);

}