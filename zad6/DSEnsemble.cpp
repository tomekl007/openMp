/*
 * DSEnsemble.cpp
 *
 *  Created on: 8 maj 2015
 *      Author: oramus
 */

#include "DSEnsemble.h"
#include <math.h>

DSEnsemble::DSEnsemble( DrunkenSailor **_ds, int _size ) {
	ds = _ds;
	size = _size;
}

DSEnsemble::~DSEnsemble() {
	// TODO Auto-generated destructor stub
}


double DSEnsemble::calcAvrDistanceFromStart() {
	double d = 0;

	for (int i = 0; i < size; i++) {
		d += ds[i]->getDistanceFromStart();
	}

	return d / size;
}

double DSEnsemble::roundAngle( double angle ) {
	double a = angle / PI2;
	if ( a == 0 ) return PI2;
	return ( a - floor( a ) ) * PI2;
}

double DSEnsemble::calcAvrAngle() {
	double sum = 0.0;

	for ( int i = 0; i < size; i++ ) {
		sum += roundAngle( ds[i]->getAngle( ) );
	}

	return sum / size;
}

void DSEnsemble::evolve( int steps, double stepSize, int trials ) {
	for (int k = 0; k < steps; k++) {
		for (int i = 0; i < size; i++)
			ds[i]->tryToMove( stepSize, trials );
		for (int i = 0; i < size; i++)
			ds[i]->copyNewPosition();
	}
}
