#include"Physics.h"
#include"Simulation.h"

#include <iostream>
#include <math.h>
#include <omp.h>

using namespace std;

const int SIZE = 2500;
const int REPETITIONS = 100;
const int STEPS_PER_REP = 2;
const int HISTOGRAM_BINS = 2500000;
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
        cout << "Liczymy bardzo duzo razy calcAngleAverageScalarProduct" << endl;
        ph->setTemperature( temperature );
 	sm->calc(0);

	for (int i = 0; i < REPETITIONS; i++) {
                sm->calcAngleAverageScalarProduct( FIELD_DIRECTION );
	}
        double res = sm->getAngleAverageScalarProduct();

        omp_set_num_threads(1);
                sm->calcAngleAverageScalarProduct( FIELD_DIRECTION );
        double res2 = sm->getAngleAverageScalarProduct();

        if ( fabs( res - res2 ) > fabs( res ) * 0.0001 ) {
          cout << endl << "!!!!!!!!!!  Rozbieznosc w liczonych srednich " << endl;
        }

	return 0;
}

