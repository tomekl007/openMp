#include"Physics.h"
#include"Simulation.h"

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <omp.h>

using namespace std;

const int SIZE = 5000;
const int REPETITIONS = 200;
const int STEPS_PER_REP = 2;
const int HISTOGRAM_BINS = 2 ;
const int HISTOGRAM_WIDTH = 100;
const int FIELD_DIRECTION = M_PI;

void showHistogram(long *h, int size, int width) {
	long max = h[0];
	for (int i = 1; i < size; i++)
		if (h[i] > max)
			max = h[i];

	cout << endl << "Maximum = " << max << endl;

	int w;
	double s = (double) width / (double) max;
	for (int i = 0; i < size; i++) {
		w = (int) (h[i] * s);
		cout << "|";
		for (int j = 0; j < w; j++)
			cout << "#";
		for ( int j = w; j < width; j++ )
			cout << " ";
		cout << "|" << endl;
	}
	cout << "|";
	for ( int i = 0; i < width; i++ )
		cout << "-" ;
	cout << "|" << endl;
}

int main(void) {
	Physics *ph = new Physics(1.0, 15.0, FIELD_DIRECTION );

	Simulation *sm = new Simulation( SIZE );

	sm->setPhysics(ph);

	double temperature = 101;
	long *hPointer;
        cout << "Liczymy bardzo dokladnie histogram" << endl;
        ph->setTemperature( temperature );
 	sm->calc(0);

// liczymy histogram sekwencyjnie i robimy jego kopie
	omp_set_num_threads(1);
        sm->calcAngleHistogram( HISTOGRAM_BINS );
	long *hPointer2 = new long[  HISTOGRAM_BINS ];
        hPointer = sm->getAngleHistogram();
        for ( int i = 0; i < HISTOGRAM_BINS; i++ )
           hPointer2[ i ] = hPointer[ i ];

	for (int i = 0; i < REPETITIONS; i++) {
		sm->calcAngleHistogram(HISTOGRAM_BINS);
	        hPointer = sm->getAngleHistogram();
	        for ( int i = 0; i < HISTOGRAM_BINS; i++ ) 
                  if ( hPointer2[ i ] != hPointer[ i ] ) {
                     cout << "!!!!!!!!!!  Blad w histogramie: powinno byc " << hPointer2[ i ] << " jest " << hPointer[ i ] << endl;
                     exit( 1 );
                  }
	}
  
        


	return 0;
}

