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
uint8_t *buffer;

uint8_t delimiter[CUBEMONSMALLBUFSIZE] = ",";
uint8_t connector[CUBEMONSMALLBUFSIZE] = ":";
uint8_t EOL[CUBEMONSMALLBUFSIZE] = ";\n";

uint8_t channelnames[CUBEMONMAXSIGNALS][CUBEMONSMALLBUFSIZE];

void CubeM_DefChNamesInit() {
	for(uint8_t i=0; i<CUBEMONMAXSIGNALS; i++) {
		sprintf((char*)channelnames[i], "Ch%d", i);
	}
}

void CubeM_init() {
	CubeM_DefChNamesInit();

	CubeM_setBufferSize(50);

	isInit = 1;
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
			buffersize = sizeb;
		}
	}
	return buffersize;
}

uint8_t CubeM_setChannelname(uint8_t channel, uint8_t *newname) {
	uint32_t templen = strlen((char*)newname);
	if((templen < CUBEMONSMALLBUFSIZE && templen > 0) && (channel > 0 && channel < CUBEMONMAXSIGNALS)) {
		strcpy((char*)channelnames[channel], (char*)newname);

		return 0;
	}
	return 1;
}

uint8_t *CubeM_getChannelname(uint8_t channel) {
	return channelnames[channel];
}

uint8_t CubeM_setBuffer(uint8_t *input, uint32_t length) {
	if(length > 0) {
		free(buffer);
		buffer = input;
		buffersize = length;
		usedbuffer = 0;
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
	if(channel > 0 && channel < CUBEMONMAXSIGNALS) {
		char tempstring[CUBEMONSMALLBUFSIZE*4];
		sprintf(tempstring, "%s%s%s%ld", delimiter, channelnames[channel], connector, value);

		uint32_t tempsize = usedbuffer + strlen((char*)tempstring);
		if(tempsize < buffersize) {
			strcat((char*)buffer, (char*)tempstring);
			usedbuffer = tempsize;
			return 0;
		}
	}
	return 1;
}

uint8_t CubeM_attendIntValue(uint8_t channel, int32_t value) {
	if(channel > 0 && channel < CUBEMONMAXSIGNALS) {
		char tempstring[CUBEMONSMALLBUFSIZE*4];
		sprintf(tempstring, "%s%s%s%ld", delimiter, channelnames[channel], connector, value);

		uint32_t tempsize = usedbuffer + strlen((char*)tempstring);
		if(tempsize < buffersize) {
			strcat((char*)buffer, (char*)tempstring);
			usedbuffer = tempsize;
			return 0;
		}
	}
	return 1;
}

uint8_t CubeM_attendFloatValue(uint8_t channel, float value, uint8_t precision) {
	if(channel > 0 && channel < CUBEMONMAXSIGNALS) {
		char tempstring[CUBEMONSMALLBUFSIZE*4];
		sprintf(tempstring, "%s%s%s%.*f", delimiter, channelnames[channel], connector, precision, value);

		uint32_t tempsize = usedbuffer + strlen((char*)tempstring);
		if(tempsize < buffersize) {
			strcat((char*)buffer, (char*)tempstring);
			usedbuffer = tempsize;
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

	CDC_Transmit_FS(buffer, usedbuffer);

	return 0;
}
