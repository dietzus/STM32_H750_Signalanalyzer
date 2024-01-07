/*
 * signalanalyzer.c
 *
 *  Created on: Jan 3, 2024
 *      Author: Martin
 */

#include "signalanalyzer.h"

siganalyzerch_t sigAnChannels[SIGANMAXCH];

uint8_t sigAn_Init() {
	for(uint8_t i=0; i<SIGANMAXCH; i++) {
		sigAnChannels[i].active = 0;
		sigAnChannels[i].buffer[0] = 0;
		sigAnChannels[i].bufsize = 10;
	}

	return 0;
}

uint8_t sigAn_setChannel(uint8_t chnumber, siganalyzerch_t channel) {
	sigAnChannels[chnumber] = channel;

	return 0;
}

siganalyzerch_t *sigAn_getChannel(uint8_t chnumber) {
	return &sigAnChannels[chnumber];
}

uint8_t sigAn_setBuffer(uint8_t chnumber, uint16_t *newbuffer, uint32_t buflen) {
//	sigAnChannels[chnumber].buffer = newbuffer;
	sigAnChannels[chnumber].bufsize = buflen;

	return 0;
}

uint16_t *sigAn_getBuffer(uint8_t chnumber) {
	return sigAnChannels[chnumber].buffer;
}

uint8_t sigAn_Debug() {
	sigAn_Init();

	return 0;
}
