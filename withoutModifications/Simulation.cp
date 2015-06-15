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

void Simulation::init( void ) {
	angle = new double* [ size ];
	angleNew = new double* [ size ];
	for ( int i = 0; i < size; i++ ) {
		angle[ i ] = new double [ size ];
		angleNew[ i ] = new double[ size ];
	}

	srand48( 12345L );
	for ( int i = 0; i < size; i++ )
		for ( int j = 0; j < size; j++ )
			angle[ i ][ j ] = ( 2.0 * drand48() - 1.0 ) * M_PI; // od -pi do +pi

	next = new int[ size ];
	previous = new int[ size ];

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

bool Simulation::useNew( double p ) {
	if ( drand48() < p ) return true;
	return false;
}

Simulation::Simulation( int _size ) : size( _size ){
	init();
}

void Simulation::setPhysics( Physics *_ph ) {
	ph = _ph;
}

void Simulation::calc( int steps ) {
	double a1, a2, a3, a4;
	double aNew;
	for ( int k = 0; k < steps; k++ ) {
		for ( int i = 0; i < size; i++ )
			for ( int j = 0; j < size; j++ ) {
				a1 = angle[ previous[ i ] ][ j ];
				a2 = angle[ next[ i ] ][ j ];
				a3 = angle[ i ][ previous[ j ] ];
				a4 = angle[ i ][ next[ j ] ];
				aNew = angle[ i ][ j ] + ( drand48() - 0.5 );
				if ( useNew( ph->getProbability( a1, a2, a3, a4, angle[ i ][ j ], aNew ) ) ) {
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
