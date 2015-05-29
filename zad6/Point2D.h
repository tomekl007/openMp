/*
 * Point2D.h
 *
 *  Created on: 6 maj 2015
 *      Author: oramus
 */

#ifndef POINT2D_H_
#define POINT2D_H_

class Point2D {
private:
	double x,y;
public:

	Point2D() {
		x = 0;
		y = 0;
	}
	Point2D( Point2D *p ) {
		x = p->x;
		y = p->y;
	}
	Point2D( double _x, double _y ) {
		x = _x;
		y = _y;
	}

	double getX() { return x; }
	double getY() { return y; }

	Point2D *newPoint( double distance, double angle );
	double distance( Point2D *p );
};

#endif /* POINT2D_H_ */
