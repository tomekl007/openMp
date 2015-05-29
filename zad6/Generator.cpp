/*
 * Generator.cpp
 *
 *  Created on: 6 maj 2015
 *      Author: oramus
 */

#include "Generator.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>

Generator::Generator() {
	delta = 0.000002;
	cdf = NULL;
	srand48( 12345L );
}

Generator::~Generator() {
	// TODO Auto-generated destructor stub
}

void Generator::setCDF(CumulativeDistributionFunction *_cdf) {
	cdf = _cdf;
}

void Generator::setDelta(double _delta) {
	delta = _delta;
}

// Wyszukuje taka liczbe vx, ze z dokladnoscia do delta, cdf->get( vx ) = x
double Generator::find(double x) {
	double xv = 0.5;
	double dx = 0.25;

	while (fabs(x - (cdf->get(xv))) > delta) {
		if (x > cdf->get(xv))
			xv += dx;
		else
			xv -= dx;
		dx *= 0.5;
	}

	return xv;
}

long *Generator::generateHistogram(long samples, int bins) {
	long *h = new long[bins];

	for (int i = 0; i < 0; i++)
		h[i] = 0;

	int bin;
	for (long i = 0; i < samples; i++) {
		bin = (int) (find(drand48()) * bins);
		h[bin]++;
	}

	return h;
}

double Generator::get() {
	return find(drand48());
}

double Generator::get( double x ) {
	return find( x );
}


void Generator::setSeed(long seed) {
	srand48(seed);
}

void Generator::selfTest() {

	int width = 80;
	int samples = 120000;
	int bins = 25;

	long *h = generateHistogram(samples, bins);

	long max = h[0];
	for (int i = 1; i < bins; i++)
		if (h[i] > max)
			max = h[i];

	std::cout << std::endl << "Maximum = " << max << std::endl;

	int w;
	double s = (double) width / (double) max;
	for (int i = 0; i < bins; i++) {
		w = (int) (h[i] * s);
		std::cout << "|";
		for (int j = 0; j < w; j++)
			std::cout << "#";
		for (int j = w; j < width; j++)
			std::cout << " ";
		std::cout << "|" << std::endl;
	}
	std::cout << "|";
	for (int i = 0; i < width; i++)
		std::cout << "-";
	std::cout << "|" << std::endl;
}

