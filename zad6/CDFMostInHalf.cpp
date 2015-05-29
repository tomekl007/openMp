/*
 * CDFMostInHalf.cpp
 *
 *  Created on: 8 maj 2015
 *      Author: oramus
 */

#include "CDFMostInHalf.h"

CDF_MostInHalf::CDF_MostInHalf() {
	// TODO Auto-generated constructor stub

}

CDF_MostInHalf::~CDF_MostInHalf() {
	// TODO Auto-generated destructor stub
}

double CDF_MostInHalf::get( double x ) {
	if ( x < 0.4 ) return x / 4.0;
	if ( x < 0.6 ) return 0.1 + ( x - 0.4 ) * 4;
	return 0.9 + ( x - 0.6 ) / 4.0;
}
