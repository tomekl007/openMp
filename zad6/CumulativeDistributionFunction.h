/*
 * CumulativeDistributionFunction.h
 *
 *  Created on: 6 maj 2015
 *      Author: oramus
 */

#ifndef CUMULATIVEDISTRIBUTIONFUNCTION_H_
#define CUMULATIVEDISTRIBUTIONFUNCTION_H_

class CumulativeDistributionFunction {
public:
	CumulativeDistributionFunction();
	virtual ~CumulativeDistributionFunction();

	virtual double get( double x );
};

#endif /* CUMULATIVEDISTRIBUTIONFUNCTION_H_ */
