#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <set>
#include <lodepng.h>
#include <string>
#include "compresor.h"

using namespace std;

#define TEST_TH 200 //372

int main() {
	if (!compress_algorithm("cat.png", "output.txt", TEST_TH)) return -1;
	if (!uncompress_algorithm("output.txt", "output.png")) return -1;
	
}