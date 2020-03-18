#include "utils.h"

// must be seeded first! returns double > 0 in success, otherwise returns 0
double db_prng(uint32_t ranmax, uint32_t db_sensibility) {
	double ans;
	if (ranmax != 0 && db_sensibility != 0) {
		ans = (rand() % ranmax + ((rand() % db_sensibility) / ((double)db_sensibility)));
	}else {
		ans = 0.0;
	}
	return ans;
}

//returns -1 in failure, the range between the two numbers must be at least of 2 integer numbers
double range_db_prng(double num1, double num2, uint32_t db_sensibility) {
	double ans;
	if (ceil(num1) >= floor(num2)) {
		ans = -1;
	}
	else {
		ans = rand() % ((uint32_t)floor(num2 - num1)) + num1 + ((rand() % db_sensibility) / ((double)db_sensibility));
	}
	return ans;
}


/*
pos_t<double> update_pos(pos_t<double> oldpos, uint32_t distance,double angulo) {
pos_t<double> newpos;
newpos.x = oldpos.x + distance * cos(angulo * PI / 180.0);
newpos.y = oldpos.y + distance * sen(angulo * PI / 180.0);
return newpos;
}
*/

