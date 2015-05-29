/*
 * CDFMostInHalf.h
 *
 *  Created on: 8 maj 2015
 *      Author: oramus
 */

#ifndef CDFMOSTINHALF_H_
#define CDFMOSTINHALF_H_

#include "CumulativeDistributionFunction.h"

class CDF_MostInHalf: public CumulativeDistributionFunction {
public:
	CDF_MostInHalf();
	virtual ~CDF_MostInHalf();
	virtual double get( double x );
};

#endif /* CDFMOSTINHALF_H_ */
