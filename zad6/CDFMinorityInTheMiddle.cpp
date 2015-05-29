/*
 * CDFMinorityInTheMiddle.cpp
 *
 *  Created on: 8 maj 2015
 *      Author: oramus
 */

#include "CDFMinorityInTheMiddle.h"

CDF_MinorityInTheMiddle::CDF_MinorityInTheMiddle() {
	// TODO Auto-generated constructor stub

}

CDF_MinorityInTheMiddle::~CDF_MinorityInTheMiddle() {
	// TODO Auto-generated destructor stub
}


double CDF_MinorityInTheMiddle::get( double x ) {
	if ( x < 0.1 ) return x * 4.0;
	if ( x < 0.9 ) return 0.4 + ( x - 0.1 ) / 4.0;
	return 0.6 + ( x - 0.9 ) * 4.0;
}
