#include"DrunkenSailor.h"
#include"CumulativeDistributionFunction.h"
#include"CDFMostInHalf.h"
#include"CDFMinorityInTheMiddle.h"
#include"DSEnsemble.h"
#include"Generator.h"
#include<iostream>
#include<math.h>

using namespace std;

const static int SAILORS = 500;
const static double PI2 = 2.0 * M_PI;
const static double SAILOR_SIZE = 0.5;
const static double STEP_SIZE = SAILOR_SIZE / 11.0;
const static int NUMBER_OF_STEPS = 1440;
const static int REPETITIONS = 10;

int main() {
	srand48( 123 );

	DrunkenSailor **ds = new DrunkenSailor*[SAILORS];
//	CumulativeDistributionFunction *cdf = new CumulativeDistributionFunction(); // bladzenie przypadkowe

//	CumulativeDistributionFunction *cdf = new CDF_MostInHalf(); // przez jakis czas idziemy w tym samym kierunku
	CumulativeDistributionFunction *cdf = new CDF_MinorityInTheMiddle(); // ciagle zmieniamy kierunek

	Generator *g = new Generator();
	g->setCDF( cdf );
	g->selfTest();
	delete g;

	double angle, dangle = PI2 / SAILORS;
	double x, y;
	double circleSize = SAILORS * SAILOR_SIZE / PI2;
	circleSize *= 1.01; // 1% extra

	// marynarze ustawieni na obwodzie okregu i ida do srodka

	for (int i = 0; i < SAILORS; i++) {
		angle = i * dangle;

		x = circleSize * cos( angle );
		y = circleSize * sin( angle );

		ds[i] = new DrunkenSailor( x, y, angle + M_PI );
//		cout << " id: " << i ;
		ds[i]->setSailors(ds, SAILORS, i, SAILOR_SIZE );
		ds[i]->setCDF( cdf );
	}

/*
    ds[0] = new DrunkenSailor( 5, 0, -M_PI );
	ds[0]->setSailors( ds, SAILORS, 0, SAILOR_SIZE );
	ds[0]->setCDF( cdf );

	ds[1] = new DrunkenSailor( -5, 0, 0 );
	ds[1]->setSailors( ds, SAILORS, 1, SAILOR_SIZE );
	ds[1]->setCDF( cdf );
*/

	DSEnsemble *dse = new DSEnsemble( ds, SAILORS );

	for ( int i = 0; i < REPETITIONS; i++ ) {
		dse->evolve( NUMBER_OF_STEPS, STEP_SIZE, 5 );

		cout << "After " << NUMBER_OF_STEPS * (i+1) << " steps" << endl;
		cout << "Average distance from starting point / step_size   : "
				<< dse->calcAvrDistanceFromStart() / STEP_SIZE << endl;
		cout << "Square root of numer of steps                      : "
				<< sqrt( NUMBER_OF_STEPS*(i+1) ) << endl;
		cout << "Average distance from starting point / sailor_size : "
				<< dse->calcAvrDistanceFromStart() / SAILOR_SIZE << endl;
		cout << "Average angle (rounded to interval 0...2pi) / pi   : "
				<< dse->calcAvrAngle() / M_PI << endl;
	}

	return 0;
}
