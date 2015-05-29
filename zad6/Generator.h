/*
 * Generator.h
 *
 *  Created on: 6 maj 2015
 *      Author: oramus
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include"CumulativeDistributionFunction.h"

class Generator {
private:
	CumulativeDistributionFunction *cdf;
	double find( double x );
	double delta;
public:
	Generator();
	virtual ~Generator();

	void setCDF( CumulativeDistributionFunction *_cdf ); // ustawienie dystrybuanty
	void setDelta( double _delta ); // dokladnosc obliczen
	void setSeed( long seed ); // ziarno dla generatora drand48

	long *generateHistogram( long samples, int bins ); // histogram
	double get(); // pojedyncza liczba losowa
	double get( double x ); // pojedyncza liczba losowa

	void selfTest( void ); // wylicza i rysuje rozklad prawdopodobienstwa
};

#endif /* GENERATOR_H_ */
