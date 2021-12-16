/*
 * utils.c
 *
 *  Created on: Dec 15, 2021
 *      Author: steelph0enix
 */


#include <stdlib.h>
#include <math.h>

double generateRandomDouble(int min, int max, unsigned precision) {
	int const precisionMultiplier = pow(10, precision);
	min *= precisionMultiplier;
	max *= precisionMultiplier;

	int const generatedValue = (rand() % (max - min)) + min;
	return ((double)generatedValue) / ((double)precisionMultiplier);
}
