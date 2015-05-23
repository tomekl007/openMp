/*
 * Simulation.h
 *
 *  Created on: 5 maj 2015
 *      Author: oramus
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include"Physics.h"

class Simulation {
private:
	double** angle;
	double** angleNew;
	Physics *ph;

	int *next; // nastepny indeks z uwzglednieniem periodycznych warunkow brzegowych
	int *previous; // poprzedni indeks z uwzglednieniem periodycznych warunkow brzegowych

	long *histogram;
	double averageAngle;

	void copyNewArray();
	bool useNew( double p,  struct drand48_data randBuffer );
	void init( void );
	const int size;
public:
	Simulation( int size );

	void setPhysics( Physics *ph );
	void calc( int steps );

	// wylicza srednia wartosc iloczynu skalarnego na podstawie
	// kierunku poszczegolnych wektorow i kierunku przekazanego w wywolaniu
	void calcAngleAverageScalarProduct( double an );

	// wylicza histogram kierunku wektorow
	void calcAngleHistogram( int bins );

	// pobiera ostatnio wyliczona wartosc srednia
	double getAngleAverageScalarProduct( void );

	// pobiera ostatnio wyliczony histogram
	long *getAngleHistogram( void );

	virtual ~Simulation();
};

#endif /* SIMULATION_H_ */
