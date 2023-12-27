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

void CubeM_DefChNamesInit();
void CubeM_init();
void CubeM_setUART();		//Currently not used
uint32_t CubeM_setBufferSize(uint32_t);
uint8_t CubeM_setChannelname(uint8_t, uint8_t *newname);
uint8_t *CubeM_getChannelname(uint8_t);
uint8_t CubeM_setBuffer(uint8_t *, uint32_t);
uint8_t CubeM_attendBuffer(uint8_t *);
uint8_t CubeM_attendUIntValue(uint8_t, uint32_t);
uint8_t CubeM_attendIntValue(uint8_t, int32_t);
uint8_t CubeM_attendFloatValue(uint8_t, float, uint8_t);
uint8_t CubeM_setDelimiter(uint8_t *);
uint8_t CubeM_setEOL(uint8_t *);
uint8_t CubeM_sendBuffer();

#if CUBEMDEBUG
uint8_t CubeM_runDebugTests();
#endif

#endif /* INC_CUBEMONITOR_H_ */
