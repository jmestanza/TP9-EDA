#include "compresor.h"

void color_data::reset(){
	min_red = 255; max_red = 0; avg_red = 0;
	min_blue = 255; max_blue = 0; avg_blue = 0;
	min_green = 255; max_green = 0; avg_green = 0;
}
void color_data::update(int i, int j, vector <unsigned char> &image, int width, int height){
	min_red = min(min_red, int(image[(i*width + j) * 4 + 0]));
	max_red = max(max_red, int(image[(i*width + j) * 4 + 0]));
	min_green = min(min_green, int(image[(i*width + j) * 4 + 1]));
	max_green = max(max_green, int(image[(i*width + j) * 4 + 1]));
	min_blue = min(min_blue, int(image[(i*width + j) * 4 + 2]));
	max_blue = max(max_blue, int(image[(i*width + j) * 4 + 2]));
	avg_red += int(image[(i*width + j) * 4 + 0]);
	avg_green += int(image[(i*width + j) * 4 + 1]);
	avg_blue += int(image[(i*width + j) * 4 + 2]);
}
void color_data::avg_update(int size) {
	if (size == 0) {
		cout << "fatal error in image compression algorithm ; \n";
		return;
	}
	avg_red /= size;
	avg_green /= size;
	avg_blue /= size;
}

void compute_average(vector <unsigned char> &image, int width, int height, int x0, int xf, int y0, int yf, color_data &ans) {
	ans.reset();
	for (int i = y0; i <= yf; i++) {
		for (int j = x0; j <= xf; j++) {
			ans.update(i, j, image, width, height);
		}
	}
	ans.avg_update((xf - x0 + 1)*(yf - y0 + 1));
}
void fill_image(vector <unsigned char> &image, int width, int height, int x0, int xf, int y0, int yf, int red, int green, int blue) {
	for (int i = y0; i <= yf; i++) {
		for (int j = x0; j <= xf; j++) {
			image[(i*width + j) * 4 + 0] = red;
			image[(i*width + j) * 4 + 1] = green;
			image[(i*width + j) * 4 + 2] = blue;
		}
	}
}
void make_color_parse(string &v, int red, int green, int blue) {
	v += ('{' + to_string(red) + ',' + to_string(green) + ',' + to_string(blue) + '}');
}
void recu(vector <unsigned char> &image, int width, int height, int th, string &data, int x0, int xf, int y0, int yf) {


	//if (x0 == xf && y0 == yf) {
		// end of recursion
		//cout << x0 << ' ' << xf << '\n';
	//	return;
	//}



	color_data ans;
	compute_average(image, width, height, x0, xf, y0, yf, ans);

	int t = ans.max_red - ans.min_red + ans.max_green - ans.min_green + ans.max_blue - ans.min_blue;
	//cout << t << ' ' << th << '\n';
	//cout << ans.max_green << ' ' << ans.min_green << '\n';
	if (t >= th && !(x0 == xf && y0 == yf)) { // demasiado no uniforme
		data += 'h'; // indicamos que entramos un nivel para adentro
		recu(image, width, height, th, data, x0, (x0 + xf) / 2, y0, (y0 + yf) / 2);
		recu(image, width, height, th, data, x0, (x0 + xf) / 2, (y0 + yf) / 2 + 1, yf);
		recu(image, width, height, th, data, (x0 + xf) / 2 + 1, xf, y0, (y0 + yf) / 2);
		recu(image, width, height, th, data, (x0 + xf) / 2 + 1, xf, (y0 + yf) / 2 + 1, yf);
	} else {
		// super uniforme!
		make_color_parse(data, ans.avg_red, ans.avg_green, ans.avg_blue); // agregamos el color al string

		//fill_image(image, width, height, x0, xf, y0, yf, ans.avg_red, ans.avg_green, ans.avg_blue);
	}
}

void generate_2_pow(set<int> &pow2, int num) {
	int j = 1;
	for (int i = 0; i < num; i++) {
		pow2.insert(j);
		j *= 2;
	}
}





/// get image, generate file
bool compress_algorithm(string image_file,string output_file, int th) {


	vector<unsigned char> image;
	unsigned width, height;
	
	if (lodepng::decode(image, width, height, image_file.c_str())) {
		cout << "error loading image '" << image_file << "' \n";
		return 0;
	}

	set<int> pow2;
	generate_2_pow(pow2, 20); // generamos todas las potencias de 2 que podemos llegar a necesitar
	if (width != height || (pow2.find(width) == pow2.end())) {
		cout << "imagen no potencia de 2 insertada\n";
		return 0;
	}
	string data;
	int v = width;
	while (v /= 2) data += 'l'; // starting header

	recu(image, width, height, th, data, 0, width - 1, 0, height - 1);

	ofstream file(output_file);
	if (!file.is_open()) {
		cout << "error opening '" << output_file << "'\n";
		file.close();
		return 0;
	}
	file << data;
	file.close();

	//lodepng::encode("test.png", image, width, height);

	return 1;
}





bool read_color(string &data, int &itr, int &red, int &green, int &blue) {
	string c1, c2, c3;
	while (data[itr] != ',' && c1.size() < 3 && itr < data.size()) c1 += data[itr++];
	itr++;
	while (data[itr] != ',' && c2.size() < 3 && itr < data.size()) c2 += data[itr++];
	itr++;
	while (data[itr] != '}' && c3.size() < 3 && itr < data.size()) c3 += data[itr++];

	if (c1.size() >= 4 || c2.size() >= 4 || c3.size() >= 4) {
		cout << "corrupt file (id 1) \n";
		return 0;
	}
	//cout << c1 << ' ' << c2 << ' ' << c3 << '\n';
	try {
		red = stoi(c1);
		green = stoi(c2);
		blue = stoi(c3);
	} catch (const std::invalid_argument& ia) {
		cout << "corrupt file (id 2) \n";
		return 0;
	}
	if (red < 0 || red >= 256 || green < 0 || green >= 256 || blue < 0 || blue >= 256) {
		cout << "corrupt file (id 2) \n";
		return 0;
	}
	return 1;
}

bool recu_uncompress(vector <unsigned char> &image, string &data, int &itr, int width, int height, int x0, int xf, int y0, int yf) {

	// hay varios casos
	if (itr == data.size()) {
		cout << "corrupt file (id 3)\n";
		return 0;
	} else if (data[itr] == '{') { // termino el nivel, hay un color
		itr++;
		int red_read, green_read, blue_read;
		if (!read_color(data, itr, red_read, green_read, blue_read)) { cout << "corrupt file (id 4)\n"; return 0; }

		fill_image(image, width, height, x0, xf, y0, yf, red_read, green_read, blue_read);
		if (itr == data.size() || data[itr] != '}') {
			cout << "corrupt file (id 5)\n";
			return 0;
		}
		itr++;
	} else if (data[itr] == 'h') { // hay un hijo, debemos iterarlo 
		itr++;
		if (!recu_uncompress(image, data, itr, width, height, x0, (x0 + xf) / 2, y0, (y0 + yf) / 2))  return 0;
		if (!recu_uncompress(image, data, itr, width, height, x0, (x0 + xf) / 2, (y0 + yf) / 2 + 1, yf)) return 0;
		if (!recu_uncompress(image, data, itr, width, height, (x0 + xf) / 2 + 1, xf, y0, (y0 + yf) / 2)) return 0;
		if (!recu_uncompress(image, data, itr, width, height, (x0 + xf) / 2 + 1, xf, (y0 + yf) / 2 + 1, yf)) return 0;

	}
	return 1;
}


bool uncompress_algorithm(string data_directory, string file_to_write) { // reconstruct image
	vector <unsigned char> image;
	
	ifstream file(data_directory);
	if (!file.is_open()) {
		cout << "error opening '" << data_directory << "'\n";
		file.close();
		return 0;
	}
	string data = "";
	while (file.good())	data += file.get();
	file.close();

	int itr = 0;
	int sz = 1; // we know size is at least 1!
	while (data[itr] == 'l') {
		sz *= 2;  // get total width/height
		itr++;
	}
	for (int i = 0; i < sz*sz * 4; i++) {
		image.push_back(255); // set initial values
	}

	if (!recu_uncompress(image, data, itr, sz, sz, 0, sz - 1, 0, sz - 1)) {
		cout << "uncompress algorithm failed \n";
		return 0;
	}

	if (lodepng::encode(file_to_write, image, sz, sz)) {
		cout << "error writing image \n";
		return 0;
	}

	return 1;
}