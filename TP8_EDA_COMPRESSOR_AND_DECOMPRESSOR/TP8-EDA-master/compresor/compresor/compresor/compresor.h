#ifndef COMPRESOR_H
#define COMRPESOR_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <set>
#include <lodepng.h>
#include <string>

using namespace std;

struct color_data {
	int min_red, max_red, avg_red;
	int min_blue, max_blue, avg_blue;
	int min_green, max_green, avg_green;
	void reset();
	void update(int i, int j, vector <unsigned char> &image, int width, int height);
	void avg_update(int size);
};

/// funciones para comprimir
bool compress_algorithm(string image_file, string output_file, int th);

void compute_average(vector <unsigned char> &image, int width, int height, int x0, int xf, int y0, int yf, color_data &ans);
void fill_image(vector <unsigned char> &image, int width, int height, int x0, int xf, int y0, int yf, int red, int green, int blue);
void make_color_parse(string &v, int red, int green, int blue);
void recu(vector <unsigned char> &image, int width, int height, int th, string &data, int x0, int xf, int y0, int yf);
void generate_2_pow(set<int> &pow2, int num);



/// funciones de descompresion
bool uncompress_algorithm(string data_directory, string file_to_write);


bool read_color(string &data, int &itr, int &red, int &green, int &blue);
bool recu_uncompress(vector <unsigned char> &image, string &data, int &itr, int width, int height, int x0, int xf, int y0, int yf);

#endif