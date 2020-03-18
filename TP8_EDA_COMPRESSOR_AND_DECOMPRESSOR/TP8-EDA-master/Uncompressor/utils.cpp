#include "utils.h"
void turn_png_negative(unsigned int &w, unsigned int&h, unsigned char * out, string path_src, string path_dst) {
	if (!lodepng_decode32_file(&out, &w, &h, &path_src[0])) {
		for (int i = 0; i < 4 * w*h; i++) {
			if (i != 0 && i % 3)
				out[i] = 0xFF - out[i];
		}
		lodepng_encode32_file(&path_dst[0], out, w, h);
		free(out);
	}
}