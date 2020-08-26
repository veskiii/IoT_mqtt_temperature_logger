/*
 * adc_calculate.h
 *
 *  Created on: Aug 25, 2020
 *      Author: Serwis-Stacjonarny
 */

#ifndef INC_ADC_CALCULATE_H_
#define INC_ADC_CALCULATE_H_

#include "math.h"

float  convertAnalogToTemperature(unsigned int analogReadValue);
float  approximateTemperatureFloat(unsigned int analogReadValue);
int approximateTemperatureInt(unsigned int analogReadValue);

#endif /* INC_ADC_CALCULATE_H_ */
