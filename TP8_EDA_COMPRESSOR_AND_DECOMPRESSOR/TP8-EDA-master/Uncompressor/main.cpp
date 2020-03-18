#include "utils.h"
#include <iostream>
#include "ev_handler.h"
#include "filesystem_header.h"
#include "allegro_init.h"
#include "image_desc.h"
#include "mosaico.h"
#include "imgs_catalog.h"
#include <boost/lexical_cast.hpp>

#define EXAMPLE "C:\\Users\\Compaq-CQ50\\Desktop"

using namespace std;

const int disp_w = 1000;
const int disp_h = 600;
const int FPS = 60;

vector<string> listall(path p);

bool parse_input(int argc, char *argv[]);

int main(int argc, char *argv[]) {

	bool input_err = parse_input(argc, argv);

	if (input_err)
	{
		return -1;
	}

	vector<string> path_of_pngs;
	if (check_existance(argv[1])) { // poner en lugar de EXAMPLE el argv[1]
		cout << ">> Empezando a buscar archivos de extension '.ajm' " << endl;
		path_of_pngs = listall(argv[1]); // tenemos los pngs en del directorio que nos pasaron!
		if (path_of_pngs.size() == 0) {
			cout << ">> No hay ningun archivo '.ajm' !" << endl;
			return -1;
		}
		cout << ">> Archivos encontrados." << endl;
	}
	else 
	{
		cout << ">> Input error: el directorio no es valido." << endl;
	}
	
	if (allegro_init())
	{
		cout << ">> Error al incializar allegro.-" << endl;
		return -1;
	}
		
	ev_handler ev_h(disp_w,disp_h,FPS);
	if (ev_h.start_and_reg())
	{
		cout << ">> Error al incializar allegro.-" << endl;
		return -1;
	}

	imgs_catalog catalogo(path_of_pngs);
	if (catalogo.is_catalog_load_error())
	{
		cout << ">> No se cargo la felcha. Revise el directorio!" << endl;
		return -1;
	}

	// mostramos la primer o unica pagina
	catalogo.draw_page(0);

	while (!ev_h.is_exit()) {
		if (ev_h.get_next_allegro_ev()) {
			ev_h.process_evs(catalogo);
		}
	}

	return 0;
}

vector<string> listall(path p) {
	directory_iterator end_itr;
	
	vector<string> found;
	// cycle through the directory
	for (directory_iterator itr(p); itr != end_itr; ++itr)
	{
		// If it's not a directory, list it. If you want to list directories too, just remove this check.
		if (is_regular_file(itr->path())) {
			// assign current file name to current_file and echo it out to the console.
			string current_file = itr->path().string();
			if (current_file.size() >= 5) { // al menos es un a.png
				if (!strcmp(&current_file[current_file.size() - 4], ".ajm")) {
					found.push_back(current_file);
					//cout << current_file << endl;
					path aux(current_file);
					//cout << aux.stem() << endl;
				}
			}	
		}
	}
	return found;
}

bool parse_input(int argc, char * argv[])
{
	if (argc > 2)
	{
		cout << ">> Input error: mas de 1 argumento ingresado. Ingrese solo 1:" << endl;
		cout << ">> (CASE SENSITIVE, Paramter 1 )" << endl;
		cout << ">>		Parameter 1 = directory" << endl;
		return true;
	}
	else if (argc < 2)
	{
		cout << ">> Input error: menos de 1 argumento ingresado. Ingrese 1 argumento:" << endl;
		cout << ">> (CASE SENSITIVE, Paramter 1)" << endl;
		cout << ">>		Parameter 1 = directory" << endl;
		return true;
	}

	return false;
}

/*
unsigned char * out=nullptr;
unsigned int w, h;
h = 934;
w = 681;

string dst = "r.png";
string src = "AshLake.png";

turn_png_negative(w, h, out,src,dst);
*/
