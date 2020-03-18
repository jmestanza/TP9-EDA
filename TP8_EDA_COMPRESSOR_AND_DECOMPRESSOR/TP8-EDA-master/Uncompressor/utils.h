#pragma once

#include <string>
#include "lodepng.h"

using namespace std;

void turn_png_negative(unsigned int &w, unsigned int&h, unsigned char * out, string path_src, string path_dst);