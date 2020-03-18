#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <cmath>
#include <iostream>

template <typename T>
struct pos_t {
	T x;
	T y;
};

double db_prng(uint32_t ranmax, uint32_t db_sensibility);
double range_db_prng(double angle1, double angle2, uint32_t db_sensibility);


#endif