/*
 * CDFMinorityInTheMiddle.h
 *
 *  Created on: 8 maj 2015
 *      Author: oramus
 */

#ifndef CDFMINORITYINTHEMIDDLE_H_
#define CDFMINORITYINTHEMIDDLE_H_

#include "CumulativeDistributionFunction.h"

class CDF_MinorityInTheMiddle: public CumulativeDistributionFunction {
public:
	CDF_MinorityInTheMiddle();
	virtual ~CDF_MinorityInTheMiddle();

	virtual double get( double x );
};

#endif /* CDFMINORITYINTHEMIDDLE_H_ */
