/*
 * app_state.h
 *
 *  Created on: Dec 15, 2021
 *      Author: steelph0enix
 */

#ifndef INC_APP_APP_STATE_H_
#define INC_APP_APP_STATE_H_

#include <stdint.h>

typedef enum AppState_t {
	APP_STATE_IDLE, APP_STATE_TRANSMITTING_DATA
} AppState;

AppState getAppState();
void setAppState(AppState newState);

double getConstantValue();
void generateConstantValue();

uint32_t getPacketNumber();
void incrementPacketNumber();
void resetPacketNumber();

#endif /* INC_APP_APP_STATE_H_ */
