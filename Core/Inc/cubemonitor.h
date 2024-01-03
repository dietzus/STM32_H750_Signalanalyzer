/*
 * cubemonitor.h
 *
 *  Created on: Dec 21, 2023
 *      Author: Martin
 */

#include "main.h"

#ifndef INC_CUBEMONITOR_H_
#define INC_CUBEMONITOR_H_

#define CUBEMDEBUG 1

#define CUBEMONMAXSIGNALS 8
#define CUBEMONSMALLBUFSIZE 20

typedef enum {
    CUBEMUINT,
    CUBEMINT,
    CUBEMFLOAT,
    CUBEMDOUBLE,
	CUBEMLDOUBLE,
	CUBEMNROFDATATYPES
} cubeMDataType;

typedef struct {
	uint8_t newvalue;
	uint8_t reuseoldvalue;
    uint8_t name[CUBEMONSMALLBUFSIZE];
    cubeMDataType datatype;
    long double value;
    int32_t precision;
} cubeMchannel_t;

void CubeM_setUART();		//Currently not used

uint8_t CubeM_setDelimiter(uint8_t *);
uint8_t CubeM_setEOL(uint8_t *);

uint32_t CubeM_defInit();
uint32_t CubeM_Init(uint32_t);
uint8_t CubeM_setBuffer(uint8_t *, uint32_t);
uint8_t CubeM_attendBuffer(uint8_t *);
uint32_t CubeM_setBufferSize(uint32_t);
uint8_t CubeM_clearBuffer();
uint8_t CubeM_sendBuffer();

void CubeM_DefChInit();
uint8_t CubeM_setChannelname(uint8_t, uint8_t *newname);
uint8_t *CubeM_getChannelname(uint8_t);
uint8_t CubeM_attendUIntValue(uint8_t, uint32_t);
uint8_t CubeM_attendIntValue(uint8_t, int32_t);
uint8_t CubeM_attendFloatValue(uint8_t, float, uint8_t);

uint8_t CubeM_setDataType(uint8_t, cubeMDataType);
uint8_t CubeM_setPrecision(uint8_t, int8_t);
#define CubeM_setValue(uint8_t, x) _Generic((x), \
		uint32_t: CubeM_setUINTValue, \
		int32_t: CubeM_setINTValue, \
		float: CubeM_setFLOATValue, \
		double: CubeM_setDOUBLEValue, \
		long double: CubeM_setLDOUBLEValue, \
        default: CubeM_setFLOATValue) (uint8_t, x)
uint8_t CubeM_sendCurValues();

#if CUBEMDEBUG
uint8_t CubeM_runDebugTests();
#endif

#endif /* INC_CUBEMONITOR_H_ */
