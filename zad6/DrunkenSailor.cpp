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
#include <omp.h>

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
	//#pragma omp parallel
	//{
		//cout << "threadNr : " << omp_get_thread_num() << endl;
		struct drand48_data randBuffer;
		long long seed = (1202107158 + (omp_get_thread_num()) * 1999);
		srand48_r(seed, &randBuffer );
		double newAngle;
		// bool abort = false;

		// #pragma omp for private(randBuffer, newAngle, abort)
		for ( int i = 0; i < trials; i++ ) {

			// #pragma omp flush (abort)	
			// if(!abort){
				// #pragma omp critical
				if ( newPosition != NULL ) delete newPosition;       // odzysk pamieci z poprzedniej iteracji petli

				double resultRand;
				drand48_r(&randBuffer, &resultRand);
				newAngle = angle + ( gen->get( resultRand ) - 0.5 ) * 2.0 * M_PI; // generowanie nowego kierunku ruchu

		//		newAngle = angle; // to wylacza losowanie kierunku - wygodne do testow!
				// #pragma omp critical
				// {
					newPosition = position->newPoint( stepSize, newAngle ); // generowanie nowego polozenia
					if ( !collision() ) { // jesli nie ma zderzenia z innym marynarzem, to OK
						angle = newAngle; // zapamietujemy nowy kierunek ruchu
						return;
						// abort = true;
						// #pragma omp flush (abort)
					}
				// }	
			}
		
		// nie udalo sie niczego wymyslec!
		// #pragma omp critical
		// {
			delete newPosition;
			newPosition = new Point2D( position );  // nowa pozycja == stara pozycja
		// }
//	}
}

