#pragma once

#ifndef ROBOTS_H
#define ROBOTS_H

typedef struct {
	double x;
	double y;
}postype;

typedef struct {
	postype pos;
	double angulo;
}robottype;

#endif