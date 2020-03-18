#include "filesystem_header.h"
int check_existance(const char* file)
{
	path p(file); //Constructor de path. lleva un const char.
	cout << endl;
	if (exists(p))
	{
		if (is_regular_file(p))
			cout << p << " existe y su tamaño es " << file_size(p) << '\n';
		else if (is_directory(p))
			cout << p << " Es un directorio!\n";
		else
			cout << p << "Es un dispositivo Pipe o un Socket. En windows, capaz la lectora de CDs\n";
		return 1;
	}
	else
		cout << p << " No existe\n";
//	getchar();
	return 0;
}

int fileSize(const char* filename)
{
	std::cout << "Tamaño de " << filename << " es " << file_size(filename) << '\n';
	getchar();
	return 0;
}
/*
file_size recibe como parámetro una clase path, que como constructor tiene un const char.
Entonces se construye en el momento de la invocación cuando se le pasa un const char a la funcion.
*/

int not_sort_directories(const char* dir)
{
	path p(dir);   // p se crea a partir de un const char
	if (exists(p))
	{
		if (is_regular_file(p))
			cout << p << " size is " << file_size(p) << '\n';
		else if (is_directory(p))
		{
			cout << p << " is a directory containing:\n";

			for (directory_iterator itr(p); itr != directory_iterator(); itr++)
				cout << itr->path().filename() << endl;
		}
	}
	else
		cout << p << "No existe\n";
	getchar();
	return 0;
}

int see_dirContent(const char* dir)
{
	path p(dir);
	if (exists(p))
	{
		if (is_regular_file(p))
			cout << p << "Es un archivo de tamaño " << file_size(p) << '\n';
		else if (is_directory(p))     // is p a directory?
		{
			cout << p << " Es un directorio, que tiene :\n \n";
			for (directory_iterator itr(p); itr != directory_iterator(); itr++)
				cout << itr->path().filename() << endl;
		}
	}
	else
		cout << p << " No existe\n";
	getchar();
	return 0;
}
/*
La funcion copy mueve todos los elementos comprendidos entre directory_iterator() (que devuelve un ITERATOR END)
y directory_iterator(p) que devuelve la carpeta raiz a  ostream_iterator, que se ocupa de transformar a la clase
especificada en <> (TEMPLATE) en algo legible para cout.
*/
#pragma warning(disable : 4996)

int returnCurrentPath() {
	cout << "\n";
	cout << current_path().string();
	cout << "\n";
	return 1;
}

int copyFile(const char* base, const char* targ) {
	copy_file(base, targ);
	return 1;
}
/*
supone que queres copiar hola.txt de la carpeta music a la carpeta imagenes. Entonces entras con {ruta}/music/hola.txt
e {ruta}/imagenes/hola.txt, aunque {ruta}/imagenes/hola.txt no exista es lo que estas creando
*/

int createDir(const char* dir) {
	return create_directory(dir);
}
#define _CRT_SECURE_NO_WARNINGS
int seeLastWritten(const char* dir) {
	char storeDate[32];

	time_t lastTime = last_write_time(dir);
	tm *readable = localtime(&lastTime);

	strftime(storeDate, 32, "%a, %d.%m.%Y %H:%M:%S", readable);
	cout << storeDate;
	return 1;
}