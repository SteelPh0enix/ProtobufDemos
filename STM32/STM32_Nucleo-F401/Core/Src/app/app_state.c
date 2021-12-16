/*
 * app_state.c
 *
 *  Created on: Dec 15, 2021
 *      Author: steelph0enix
 */

#include "app/app_state.h"
#include "app/utils.h"

#include <stdlib.h>

static AppState currentAppState = APP_STATE_IDLE;
static double constantVal = 0;
static uint32_t packetNumber = 0;

AppState getAppState() {
	return currentAppState;
}

void setAppState(AppState newState) {
	// in case when we begin data output...
	if (currentAppState == APP_STATE_IDLE && newState == APP_STATE_TRANSMITTING_DATA) {
		resetPacketNumber();
	}

	currentAppState = newState;
}

double getConstantValue() {
	return constantVal;
}

void generateConstantValue() {
	constantVal = generateRandomDouble(0, 10000, 3);
}

uint32_t getPacketNumber() {
	return packetNumber;
}

void incrementPacketNumber() {
	packetNumber++;
}

void resetPacketNumber() {
	packetNumber = 0;
}
