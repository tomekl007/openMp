#include "Physics.h"
#include <math.h>

Physics::Physics( double _J, double _H, double _HA ) : J(_J), H(_H), HA(_HA ) {
	 temperature = 1.0;
}

Physics::Physics() : J(1.0), H(0.0), HA(0.0) {
	 temperature = 1.0;
}

double Physics::glauber( double dE ) {
	double e = exp( -dE / temperature );
	return e / ( 1 + e );
}

double Physics::getProbability( double a1, double a2, double a3, double a4, double aOld, double aNew ) {
	double oldE = - J * ( cos( a1 - aOld ) + cos( a2 - aOld ) + cos( a3 - aOld ) + cos( a4 - aOld ) ) -
			H * cos( HA - aOld );
	double newE = - J * ( cos( a1 - aNew ) + cos( a2 - aNew ) + cos( a3 - aNew ) + cos( a4 - aNew ) ) -
			H * cos( HA - aNew );
	return glauber( newE - oldE );
}

void Physics::setTemperature( double t ) {
	temperature = t;
}
