/*
 * Point2D.cpp
 *
 *  Created on: 6 maj 2015
 *      Author: oramus
 */

#include "Point2D.h"
#include <math.h>

double Point2D::distance(Point2D *p) {
	double dx = x - p->x;
	double dy = y - p->y;
	return sqrt(dx * dx + dy * dy);
}

Point2D* Point2D::newPoint(double distance, double angle) {
	return new Point2D(x + distance * cos(angle), y + distance * sin(angle));
}

