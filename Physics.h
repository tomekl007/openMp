#ifndef _PHYSICS_H_

#define _PHYSICS_H_

class Physics {
private:
	const double J;
	const double H;
	const double HA;
	double temperature;

	double glauber( double dE );
public:
	double getProbability( double a1, double a2, double a3, double a4, double aOld, double aNew );
	void setTemperature( double t );
	Physics( double _J, double _H, double _HA );
	Physics();
};


#endif
