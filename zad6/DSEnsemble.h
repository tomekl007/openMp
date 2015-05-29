/*
 * DSEnsamble.h
 *
 *  Created on: 8 maj 2015
 *      Author: oramus
 */

#ifndef DSENSEMBLE_H_
#define DSENSEMBLE_H_

#include"DrunkenSailor.h"
#include<math.h>

class DSEnsemble {
private:
	DrunkenSailor **ds; // tablica wskaznikow do obiektow-marynarzy
	int size; // liczba marynarzy

	double roundAngle( double x );
	static const double PI2 = 2.0 * M_PI;
public:
	DSEnsemble( DrunkenSailor **_ds, int size );
	virtual ~DSEnsemble();

	void evolve( int steps, double stepSize, int trials );
	double calcAvrDistanceFromStart();
	double calcAvrAngle();
};

#endif /* DSENSEMBLE_H_ */
