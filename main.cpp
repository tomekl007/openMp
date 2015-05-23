#include"Physics.h"
#include"Simulation.h"

#include <iostream>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

const int SIZE = 200;
const int REPETITIONS = 20;
const int STEPS_PER_REP = 200;
const int HISTOGRAM_BINS = 25;
const int HISTOGRAM_WIDTH = 100;
const int FIELD_DIRECTION = M_PI;



void show( struct timeval *ti ) {
  struct timeval tf;
  static double last = 0.0;
  gettimeofday( &tf, NULL );
  double now = tf.tv_sec + tf.tv_usec * 0.000001;
  double ini = ti->tv_sec - ti->tv_usec * 0.000001;
  if ( last > 0.0 )
     printf( "--- Time report: %8.5lf ( + %09.6lf )\n", now - ini , now - last ); 
  else
     printf( "--- Time report: %8.5lf ( + %09.6lf )\n", now - ini , now - ini ); 
  last = now;
} 


void showHistogram(long *h, int size, int width) {
	long max = h[0];
	for (int i = 1; i < size; i++)
		if (h[i] > max)
			max = h[i];

	cout << endl << "Maximum = " << max << endl;

	int w;
	double s = (double) width / (double) max;
	#pragma omp parallel for schedule( static, 5 )
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

  	struct timeval ti;
  	gettimeofday( &ti, NULL );


	Physics *ph = new Physics(1.0, 15.0, FIELD_DIRECTION );

	Simulation *sm = new Simulation( SIZE );

	sm->setPhysics(ph);

	double temperature = 101;
	long *hPointer;
	for (int i = 0; i < REPETITIONS; i++) {
		temperature -= 5;
		cout << "Temperatura ........................................ "
				<< temperature << endl;
		ph->setTemperature( temperature );

		sm->calc(STEPS_PER_REP);

		sm->calcAngleAverageScalarProduct( FIELD_DIRECTION );
		cout << "Test zgodnosci ustawienia dla kierunku zgodnego      "
				<< sm->getAngleAverageScalarProduct() << endl;

		sm->calcAngleAverageScalarProduct( FIELD_DIRECTION + M_PI_2 );
		cout << "Test zgodnosci ustawienia dla kierunku prostopadlego "
				<< sm->getAngleAverageScalarProduct() << endl;

		sm->calcAngleHistogram(HISTOGRAM_BINS);
		hPointer = sm->getAngleHistogram();

		showHistogram( hPointer, HISTOGRAM_BINS, HISTOGRAM_WIDTH );
		cout << endl;
	}

	 show( &ti ); 

	return 0;
}

