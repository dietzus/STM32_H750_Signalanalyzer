/*
 * cubemonitor.c
 *
 *  Created on: Dec 21, 2023
 *      Author: Martin
 */

#include "cubemonitor.h"

#include <stdlib.h>
#include "string.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

uint8_t isInit = 0;

uint8_t useUART = 0;

uint32_t buffersize = 0;
uint32_t usedbuffer = 0;
uint8_t emptybuffer = 1;
uint8_t *buffer;

uint8_t delimiter[CUBEMONSMALLBUFSIZE] = ",";
uint8_t dellen = 0;
uint8_t connector[CUBEMONSMALLBUFSIZE] = ":";
uint8_t conlen = 0;
uint8_t EOL[CUBEMONSMALLBUFSIZE] = "\n";
uint8_t EOLlen = 0;

cubeMchannel_t cubemChannels[CUBEMONMAXSIGNALS];

void CubeM_DefChInit() {
	for(uint8_t i=0; i<CUBEMONMAXSIGNALS; i++) {
		cubemChannels[i].newvalue = 0;
		cubemChannels[i].reuseoldvalue = 0;
		cubemChannels[i].datatype = 2;
		sprintf((char*)(cubemChannels[i].name), "Ch%d", i);
		cubemChannels[i].value = 0.f;
	}
}

void CubeM_getStrLengths() {
	dellen = strlen((char*)delimiter);
	conlen = strlen((char*)connector);
	EOLlen = strlen((char*)EOL);
}

uint32_t CubeM_defInit() {
	return CubeM_Init(50);
}

uint32_t CubeM_Init(uint32_t bufsize) {
	CubeM_DefChInit();

	uint32_t tempsize = CubeM_setBufferSize(bufsize);

	if(tempsize == 0 || tempsize!=bufsize) {
		tempsize = CubeM_setBufferSize(50);
	}

	CubeM_getStrLengths();
	isInit = 1;

	return tempsize;
}

//Currently not used
void CubeM_setUART() {

}

uint32_t CubeM_setBufferSize(uint32_t sizeb) {
	if(sizeb != buffersize) {
		uint8_t *temppointer = malloc(sizeb);
		if(temppointer != NULL) {
			free(buffer);
			buffer = temppointer;
			buffer[0] = '\0';

			buffersize = sizeb;
			emptybuffer = 1;
			usedbuffer = 0;
		}
	}
	return buffersize;
}

uint8_t CubeM_clearBuffer() {
	if(emptybuffer == 1 && usedbuffer == 0) return 1;

	buffer[0] = '\0';
	emptybuffer = 1;
	usedbuffer = 0;
	return 0;
}

uint8_t CubeM_setChannelname(uint8_t channel, uint8_t *newname) {
	uint32_t templen = strlen((char*)newname);
	if((templen < CUBEMONSMALLBUFSIZE && templen > 0) && (channel >= 0 && channel < CUBEMONMAXSIGNALS)) {
		strcpy((char*)(cubemChannels[channel].name), (char*)newname);

		return 0;
	}
	return 1;
}

uint8_t *CubeM_getChannelname(uint8_t channel) {
	return cubemChannels[channel].name;
}

uint8_t CubeM_setBuffer(uint8_t *input, uint32_t length) {
	if(length > 0) {
		free(buffer);
		buffer = input;
		buffersize = length;
		uint32_t templen = strlen((char*)input);
		if(templen == 0) {
			emptybuffer = 1;
			usedbuffer = 0;
		} else {
			emptybuffer = 0;
			usedbuffer = templen;
		}

		return 0;
	}
	return 1;
}

uint8_t CubeM_attendBuffer(uint8_t *input) {
	uint32_t tempsize = usedbuffer + strlen((char*)input);
	if(tempsize < buffersize) {
		strcat((char*)buffer, (char*)input);
		usedbuffer = tempsize;
		return 0;
	}

	return 1;
}

uint8_t CubeM_attendUIntValue(uint8_t channel, uint32_t value) {
	if(channel >= 0 && channel < CUBEMONMAXSIGNALS) {
		char tempstring[CUBEMONSMALLBUFSIZE*4];

		uint32_t tempsize;
		sprintf(tempstring, "%s%s%s%ld", delimiter, cubemChannels[channel].name, connector, value);
		tempsize = usedbuffer + strlen((char*)tempstring) - (emptybuffer * dellen);

		if(tempsize < buffersize) {
			strcpy((char*)(buffer + (usedbuffer)), (char*)(tempstring + (emptybuffer * dellen)));
			usedbuffer = tempsize;
			emptybuffer = 0;
			return 0;
		}
	}
	return 1;
}

uint8_t CubeM_attendIntValue(uint8_t channel, int32_t value) {
	if(channel > 0 && channel < CUBEMONMAXSIGNALS) {
		char tempstring[CUBEMONSMALLBUFSIZE*4];

		uint32_t tempsize;
		sprintf(tempstring, "%s%s%s%ld", delimiter, cubemChannels[channel].name, connector, value);
		tempsize = usedbuffer + strlen((char*)tempstring) - (emptybuffer * dellen);

		if(tempsize < buffersize) {
			strcpy((char*)(buffer + (usedbuffer)), (char*)(tempstring + (emptybuffer * dellen)));
			usedbuffer = tempsize;
			emptybuffer = 0;
			return 0;
		}
	}
	return 1;
}

uint8_t CubeM_attendFloatValue(uint8_t channel, float value, uint8_t precision) {
	if(channel > 0 && channel < CUBEMONMAXSIGNALS) {
		char tempstring[CUBEMONSMALLBUFSIZE*4];

		uint32_t tempsize;
		sprintf(tempstring, "%s%s%s%.*f", delimiter, cubemChannels[channel].name, connector, precision, value);
		tempsize = usedbuffer + strlen((char*)tempstring) - (emptybuffer * dellen);

		if(tempsize < buffersize) {
			strcpy((char*)(buffer + (usedbuffer)), (char*)(tempstring + (emptybuffer * dellen)));
			usedbuffer = tempsize;
			emptybuffer = 0;
			return 0;
		}
	}
	return 1;
}

uint8_t CubeM_setDelimiter(uint8_t *string) {
	if(strlen((char*)string) < CUBEMONSMALLBUFSIZE) {
		return 1;
	}
	strcpy((char*)delimiter, (char*)string);

	return 0;
}

uint8_t CubeM_setEOL(uint8_t *string) {
	if(strlen((char*)string) < CUBEMONSMALLBUFSIZE) {
		return 1;
	}
	strcpy((char*)EOL, (char*)string);

	return 0;
}

uint8_t CubeM_sendBuffer() {

	if(emptybuffer == 1) return 1;

	strcpy((char*)&buffer[usedbuffer], (char*)EOL);
	CDC_Transmit_FS(buffer, usedbuffer);

	return 0;
}

uint8_t CubeM_setDataType(uint8_t channel, cubeMDataType newtype) {
	if(channel >= CUBEMONMAXSIGNALS || newtype >= CUBEMNROFDATATYPES) return 1;

	cubemChannels[channel].datatype = newtype;
	return 0;
}

uint8_t CubeM_setPrecision(uint8_t channel, int8_t precision) {
	if(channel >= CUBEMONMAXSIGNALS) return 1;

	cubemChannels[channel].precision = precision;
	return 0;
}

uint8_t CubeM_setUINTValue(uint8_t channel, float value) {

	if(channel >= CUBEMONMAXSIGNALS || cubemChannels[channel].datatype != CUBEMUINT) return 1;
	cubemChannels[channel].value = value;
	cubemChannels[channel].newvalue = 1;
	return 0;
}

uint8_t CubeM_setINTValue(uint8_t channel, float value) {

	if(channel >= CUBEMONMAXSIGNALS || cubemChannels[channel].datatype != CUBEMINT) return 1;
	cubemChannels[channel].value = value;
	cubemChannels[channel].newvalue = 1;
	return 0;
}

uint8_t CubeM_setFLOATValue(uint8_t channel, float value) {

	if(channel >= CUBEMONMAXSIGNALS || cubemChannels[channel].datatype != CUBEMFLOAT) return 1;
	cubemChannels[channel].value = value;
	cubemChannels[channel].newvalue = 1;
	return 0;
}

uint8_t CubeM_setDOUBLEValue(uint8_t channel, double value) {

	if(channel >= CUBEMONMAXSIGNALS || cubemChannels[channel].datatype != CUBEMDOUBLE) return 1;
	cubemChannels[channel].value = value;
	cubemChannels[channel].newvalue = 1;
	return 0;
}

uint8_t CubeM_setLDOUBLEValue(uint8_t channel, long double value) {

	if(channel >= CUBEMONMAXSIGNALS || cubemChannels[channel].datatype != CUBEMLDOUBLE) return 1;
	cubemChannels[channel].value = value;
	cubemChannels[channel].newvalue = 1;
	return 0;
}

uint8_t CubeM_sendCurValues() {
	CubeM_clearBuffer();

	uint8_t tempbuffer[CUBEMONSMALLBUFSIZE*4];
	uint8_t datatowrite = 0;
	uint32_t tempsize = 0;

	for(uint8_t i=0; i<CUBEMONMAXSIGNALS; i++) {
		if(cubemChannels[i].newvalue == 0) continue;

		switch(cubemChannels[i].datatype) {
		case CUBEMUINT:
			sprintf((char*)tempbuffer, "%s%s%s%ld", delimiter, cubemChannels[i].name, connector, (uint32_t)cubemChannels[i].value);
			break;
		case CUBEMINT:
			sprintf((char*)tempbuffer, "%s%s%s%ld", delimiter, cubemChannels[i].name, connector, (int32_t)cubemChannels[i].value);
			break;
		case CUBEMFLOAT:
			sprintf((char*)tempbuffer, "%s%s%s%.*f", delimiter, cubemChannels[i].name, connector, (int)cubemChannels[i].precision, (float)cubemChannels[i].value);
			break;
		case CUBEMDOUBLE:
			sprintf((char*)tempbuffer, "%s%s%s%.*lf", delimiter, cubemChannels[i].name, connector, (int)cubemChannels[i].precision, (double)cubemChannels[i].value);
			break;
		case CUBEMLDOUBLE:
			sprintf((char*)tempbuffer, "%s%s%s%.*Lf", delimiter, cubemChannels[i].name, connector, (int)cubemChannels[i].precision, (long double)cubemChannels[i].value);
			break;
		default:
			continue;
		}

		tempsize = usedbuffer + strlen((char*)tempbuffer) - (emptybuffer * dellen);

		if(tempsize < buffersize) {
			strcpy((char*)(buffer + (usedbuffer)), (char*)(tempbuffer + (emptybuffer * dellen)));
			datatowrite = 1;
			usedbuffer = tempsize;
			emptybuffer = 0;
		}
		cubemChannels[i].newvalue = cubemChannels[i].reuseoldvalue;
	}

	if(datatowrite == 1) {
		CubeM_sendBuffer();
	}

	return 0;
}

#if CUBEMDEBUG

void CubeM_DEBUGERRORHANDLER(uint32_t errorcode) {
	asm("NOP");
}

uint8_t CubeM_runDebugTests() {

	uint32_t errornumber = 0;

	uint32_t tempbuffersize = 100;
	if(CubeM_Init(tempbuffersize) != tempbuffersize) CubeM_DEBUGERRORHANDLER(errornumber);

	errornumber++;
	if(CubeM_setChannelname(0, (uint8_t*)"Test0") != 0) CubeM_DEBUGERRORHANDLER(errornumber);

	errornumber++;
	if(CubeM_attendUIntValue(0, 123) != 0) CubeM_DEBUGERRORHANDLER(errornumber);

	errornumber++;
	if(CubeM_attendUIntValue(0, 234) != 0) CubeM_DEBUGERRORHANDLER(errornumber);

	errornumber++;
	if(CubeM_attendUIntValue(1, 123) != 0) CubeM_DEBUGERRORHANDLER(errornumber);

	errornumber++;
	if(CubeM_attendUIntValue(1, 234) != 0) CubeM_DEBUGERRORHANDLER(errornumber);

	errornumber++;
	if(CubeM_attendIntValue(1, -123) != 0) CubeM_DEBUGERRORHANDLER(errornumber);

	errornumber++;
	if(CubeM_attendFloatValue(2, 0.123, 3) != 0) CubeM_DEBUGERRORHANDLER(errornumber);

	errornumber++;
	if(CubeM_attendFloatValue(2, 0.123, 2) != 0) CubeM_DEBUGERRORHANDLER(errornumber);

	uint8_t debugchnumbers[] = {0, 1, 2, 3, 4};
	cubeMDataType debugdatatypes[] = {CUBEMUINT, CUBEMINT, CUBEMFLOAT, CUBEMDOUBLE, CUBEMLDOUBLE};
	uint8_t debugprecisionvalues[] = {2, 3, 4, 5, 6};

	for(uint8_t i=0; i<CUBEMNROFDATATYPES; i++) {
		errornumber++;
		if(CubeM_setDataType(debugchnumbers[i], debugdatatypes[i])!= 0) CubeM_DEBUGERRORHANDLER(errornumber);

		errornumber++;
		if(cubemChannels[debugchnumbers[i]].datatype != debugdatatypes[i]) CubeM_DEBUGERRORHANDLER(errornumber);

		errornumber++;
		if(CubeM_setPrecision(debugchnumbers[i], debugprecisionvalues[i])!= 0) CubeM_DEBUGERRORHANDLER(errornumber);
	}

	errornumber++;
	if(CubeM_setValue(debugchnumbers[0], (uint32_t)123) != 0) CubeM_DEBUGERRORHANDLER(errornumber);

	errornumber++;
	if(CubeM_setValue(debugchnumbers[1], (int32_t)-123) != 0) CubeM_DEBUGERRORHANDLER(errornumber);

	errornumber++;
	if(CubeM_setValue(debugchnumbers[2], (float)0.123456789) != 0) CubeM_DEBUGERRORHANDLER(errornumber);

	errornumber++;
	if(CubeM_setValue(debugchnumbers[3], (double)0.123456789) != 0) CubeM_DEBUGERRORHANDLER(errornumber);

	errornumber++;
	if(CubeM_setValue(debugchnumbers[4], (long double)0.123456789) != 0) CubeM_DEBUGERRORHANDLER(errornumber);

	errornumber++;
	if(CubeM_setValue(debugchnumbers[0], (float)0.123456789) != 1) CubeM_DEBUGERRORHANDLER(errornumber);

	errornumber++;
	if(CubeM_sendCurValues() != 0) CubeM_DEBUGERRORHANDLER(errornumber);

	uint8_t tempstatus = 0;
	uint32_t tempcounter = 0;
	while(tempstatus == 0) {
		tempstatus = CubeM_attendFloatValue(2, 0.123+tempcounter, 2);
		tempcounter++;
	}

	errornumber++;
	if(usedbuffer == 0 || emptybuffer == 1 || buffer[0] == '\0') CubeM_DEBUGERRORHANDLER(errornumber);

	HAL_Delay(3000);
	CubeM_sendBuffer();

	errornumber++;
	if(CubeM_clearBuffer() != 0) CubeM_DEBUGERRORHANDLER(errornumber);

	errornumber++;
	if(usedbuffer != 0 || emptybuffer != 1 || buffer[0] != '\0') CubeM_DEBUGERRORHANDLER(errornumber);

	return 0;
}
#endif
