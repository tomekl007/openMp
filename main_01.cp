#include"Physics.h"
#include"Simulation.h"

#include <iostream>
#include <math.h>

using namespace std;

const int SIZE = 300;
const int REPETITIONS = 20;
const int STEPS_PER_REP = 200;
const int HISTOGRAM_BINS = 25;
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
	ph->setTemperature( temperature );

	cout << "TYLKO OBLICZENIA metoda CALC" << endl;
	for ( int i = 0; i < REPETITIONS; i++ )
   	   sm->calc(STEPS_PER_REP);

	return 0;
}

