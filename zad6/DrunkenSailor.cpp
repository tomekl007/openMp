/*
 * DrunkenSailor.cpp
 *
 *  Created on: 6 maj 2015
 *      Author: oramus
 */

#include "DrunkenSailor.h"
#include <cstddef>
#include <math.h>
#include <iostream>

using namespace std;

DrunkenSailor::DrunkenSailor( double x, double y, double _angle ) {
	startPosition = new Point2D( x, y );
	position = new Point2D( x, y );
	angle = _angle;
	// to ponizej to smieci dla uspokojenia kompilatora
	newPosition = NULL;
	size = 0;
	sailors = NULL;
	id = -1;
	sailorSize = 0;
	gen = new Generator();
}

DrunkenSailor::~DrunkenSailor() {
	delete startPosition;
	delete position;
	if ( newPosition != NULL ) delete newPosition;
	delete gen;
}

bool DrunkenSailor::collision() {
	for ( int i = 0; i < id; i++ ) {
		if ( newPosition->distance( sailors[i]->position )  < sailorSize )
			return true;
	}
	for ( int i = id+1; i < size; i++ ) {
		if ( newPosition->distance( sailors[i]->position )  < sailorSize )
			return true;
	}
	return false;
}

void DrunkenSailor::tryToMove( double stepSize, int trials ) {
	double newAngle;
	for ( int i = 0; i < trials; i++ ) {
		if ( newPosition != NULL ) delete newPosition;       // odzysk pamieci z poprzedniej iteracji petli
		newAngle = angle + ( gen->get( drand48() ) - 0.5 ) * 2.0 * M_PI; // generowanie nowego kierunku ruchu

//		newAngle = angle; // to wylacza losowanie kierunku - wygodne do testow!

		newPosition = position->newPoint( stepSize, newAngle ); // generowanie nowego polozenia
		if ( !collision() ) { // jesli nie ma zderzenia z innym marynarzem, to OK
			angle = newAngle; // zapamietujemy nowy kierunek ruchu
			return;
		}
	}
	// nie udalo sie niczego wymyslec!
	delete newPosition;
	newPosition = new Point2D( position );  // nowa pozycja == stara pozycja
}

