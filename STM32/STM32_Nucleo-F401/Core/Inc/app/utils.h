/*
 * utils.h
 *
 *  Created on: Dec 15, 2021
 *      Author: steelph0enix
 */

#ifndef INC_APP_UTILS_H_
#define INC_APP_UTILS_H_

// IMPORTANT: this function DOES NOT check for integer overflow.
// Since only random value we can generate is integer, it'll increase
// the min and max range to fit the precision requirement. Make sure
// the limite of range (min*10^precision, max*10^precision) will not overflow 32 bits
double generateRandomDouble(int min, int max, unsigned precision);

#endif /* INC_APP_UTILS_H_ */
