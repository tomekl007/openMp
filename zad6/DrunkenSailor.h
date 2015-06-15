/*
 * DrunkenSailor.h
 *
 *  Created on: 6 maj 2015
 *      Author: oramus
 */

#ifndef DRUNKENSAILOR_H_
#define DRUNKENSAILOR_H_

#include <stdlib.h>
#include <iostream>
#include "Point2D.h"
#include "Generator.h"
#include "CumulativeDistributionFunction.h"

class DrunkenSailor {
private:
	Point2D *position;
	Point2D *newPosition;
	Point2D *startPosition;
	Generator *gen;

	struct drand48_data randBuffer;
	double angle;
	int id; // identyfikator marynarza
	int size; // rozmiar tablicy z marynarzami
	DrunkenSailor **sailors; // towarzystwo
	double sailorSize; // wielkosc marynarza

	bool collision(); // zderzenie z innym marynarzem.
public:
	DrunkenSailor( double x, double y, double angle );
	void setSailors( DrunkenSailor** _sailors, int _size, int _id, double _sailorSize ) {
		sailors = _sailors;
		size = _size;
		id = _id;
		sailorSize = _sailorSize;
		srand48_r(1234L + id * 4015, &randBuffer);
	}

	void setCDF( CumulativeDistributionFunction * cdf ) {
		gen->setCDF( cdf );
	}

	void tryToMove( double stepSize, int trials ); // generowanie nowego polozenia
	void copyNewPosition() {
		delete position;  // odzysk pamieci
		position = newPosition;
		newPosition = NULL;  // nowa pozycja juz nie jest poprawna
	}

	double getX() { return position->getX(); }
	double getY() { return position->getY(); }
	double getDistanceFromStart() { return startPosition->distance( position ); }
	double getAngle() { return angle; }

	virtual ~DrunkenSailor();
};

#endif /* DRUNKENSAILOR_H_ */
