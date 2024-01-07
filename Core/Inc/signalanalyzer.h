/*
 * signalanalyzer.h
 *
 *  Created on: Jan 3, 2024
 *      Author: Martin
 */

#ifndef INC_SIGNALANALYZER_H_
#define INC_SIGNALANALYZER_H_

#include "main.h"

#include "adc.h"

#define SIGANDEBUG 1

#define SIGANMAXCH 3

typedef struct {
	uint8_t active;
	adc_t adc;
	uint16_t buffer[10000];
	uint32_t bufsize;
} siganalyzerch_t;

uint8_t sigAn_Init();



uint8_t sigAn_Debug();

#endif /* INC_SIGNALANALYZER_H_ */
