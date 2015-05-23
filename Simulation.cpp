/*
 * Simulation.cpp
 *
 *  Created on: 5 maj 2015
 *      Author: oramus
 */

#include "Simulation.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;


void show( drand48_data *d ) {
#pragma omp critical 
{
   cout << "__x[0] = " << d->__x[0] << endl;
   cout << "__x[1] = " << d->__x[1] << endl;
   cout << "__x[2] = " << d->__x[2] << endl;
   
   cout << "__old_x[0] = " << d->__old_x[0] << endl;
   cout << "__old_x[1] = " << d->__old_x[1] << endl;
   cout << "__old_x[2] = " << d->__old_x[2] << endl;
   
   cout << "__c    = " << d->__c << endl;
   cout << "__init = " << d->__init << endl;
   cout << "__a    = " << d->__a << endl;
} // critical
}

void Simulation::init( void ) {
	double randResult;
 	struct drand48_data randBuffer;
 	srand48_r(time(NULL), &randBuffer );

	angle = new double* [ size ];
	angleNew = new double* [ size ];
	for ( int i = 0; i < size; i++ ) {
		angle[ i ] = new double [ size ];
		angleNew[ i ] = new double[ size ];
	}

	
	//it will execute as many times as OMP_NUM_THREADS={nrOfTimes}
	// #pragma omp parallel firstprivate( randBuffer )
	// {
	//    show( &randBuffer );
	// } // parallel



 	#pragma omp parallel for schedule( dynamic ) private(randBuffer)
 	for ( int i = 0; i < size; i++ )
		for ( int j = 0; j < size; j++ ){
			double randResult;
 			drand48_r(&randBuffer, &randResult);
			angle[ i ][ j ] = ( 2.0 * randResult - 1.0 ) * M_PI; // od -pi do +pi
		}

	next = new int[ size ];
	previous = new int[ size ];

 	#pragma omp parallel for schedule( static ) private(randBuffer)
	for ( int i = 1; i < size - 1; i++ ) {
		next[ i ] = i + 1;
		previous[ i ] = i - 1;
	}

	next[ 0 ] = 1;
	previous[ 0 ] = size - 1;

	next[ size - 1 ] = 0;
	previous[ size - 1 ] = size - 2;

	histogram = NULL;
}

void Simulation::copyNewArray() {
	for ( int i = 0; i < size; i++ )
		for ( int j = 0; j < size; j++ )
			angle[ i ][ j ] = angleNew[ i ][ j ];
}

bool Simulation::useNew( double p, struct drand48_data randBuffer) {
	double randResult;
 	drand48_r(&randBuffer, &randResult);
 	if ( randResult < p ) return true;
	return false;
}

Simulation::Simulation( int _size ) : size( _size ){
	init();
}

void Simulation::setPhysics( Physics *_ph ) {
	ph = _ph;
}

void Simulation::calc( int steps ) {
	struct drand48_data randBuffer;
 	srand48_r(time(NULL), &randBuffer );
	double a1, a2, a3, a4;
	double aNew;
	#pragma omp parallel for schedule( static ) private(randBuffer)
	for ( int k = 0; k < steps; k++ ) {
		for ( int i = 0; i < size; i++ )
			for ( int j = 0; j < size; j++ ) {
				a1 = angle[ previous[ i ] ][ j ];
				a2 = angle[ next[ i ] ][ j ];
				a3 = angle[ i ][ previous[ j ] ];
				a4 = angle[ i ][ next[ j ] ];
				double randResult;
				drand48_r(&randBuffer, &randResult);
				aNew = angle[ i ][ j ] + ( randResult - 0.5 );
				if ( useNew( ph->getProbability( a1, a2, a3, a4, angle[ i ][ j ], aNew ) , randBuffer) ) {
					angleNew[ i ][ j ] = aNew;
				} else {
					angleNew[ i ][ j ] = angle[ i ][ j ];
				}
			} // i,j
		copyNewArray(); // angleNew -> angle
	} // k
}

void Simulation::calcAngleHistogram( int bins ) {
	double pi2 = 2.0 * M_PI;
	double s = bins / pi2;

	if ( histogram != NULL ) delete histogram;

	histogram = new long[ bins ];
	for ( int i = 0; i < bins; i++ )
		histogram[ i ] = 0L;

	int bin;
	int mult;
	for ( int i = 0; i < size; i++ )
		for ( int j = 0; j < size; j++ ) {
			mult = (int)( angle[ i ][ j ] / pi2 );
			if ( angle[i][j] < 0 ) mult--;
			bin = s * ( angle[ i ][ j ] - mult * pi2 );
			if ( bin < 0.0 ) {
				bin = 0;
				cout << "L!" << endl;  // tak na wszelki wypadek
			}
			if ( bin > ( size - 1 ) ) {
				bin = size - 1;
				cout << "H!" << endl; // takze na wszelki wypadek
			}
			histogram[ bin ]++;
		}
}

long *Simulation::getAngleHistogram() {
	return histogram;
}

double Simulation::getAngleAverageScalarProduct() {
	return averageAngle;
}

void Simulation::calcAngleAverageScalarProduct( double an ) {
	double sum = 0.0;

	for ( int i = 0; i < size; i++ )
		for ( int j = 0; j < size; j++ )
			sum += cos( angle[ i ][ j ] - an );

	averageAngle = sum / ( size * size );
}

Simulation::~Simulation() {
	// TODO Auto-generated destructor stub
}
