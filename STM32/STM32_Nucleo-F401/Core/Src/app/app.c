/*
 * app.c
 *
 *  Created on: Dec 15, 2021
 *      Author: steelph0enix
 */
#include "app/app.h"
#include "app/message_handler.h"
#include "app/app_state.h"
#include "app/data_generator.h"
#include "app/bit_helpers.h"
#include "main.h"
#include "usart.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Used UART HAL handle
#define UART_HANDLE huart2
// Size of UART data buffers, in bytes
#define UART_DATA_BUFFER_SIZE 256
// Size of length field of our Protobuf packet, in bytes
#define HEADER_SIZE_LENGTH 2
// Seed for RNG
#define RANDOM_SEED 647891196

static uint8_t uartRequestBuffer[UART_DATA_BUFFER_SIZE] = { 0 };
static uint16_t uartRequestLength = 0;

static uint8_t uartResponseBuffer[UART_DATA_BUFFER_SIZE] = { 0 };

static uint32_t dataDelay = 200;
static uint32_t lastDataSentTimestamp = 0;

void setSizeHeader(uint8_t* buffer, uint16_t size);
uint16_t getSizeHeader(uint8_t* buffer);

void appSetup() {
	srand(RANDOM_SEED);
	generateConstantValue();
	printf("   \r\nHello, world!\r\n");

	HAL_UARTEx_ReceiveToIdle_DMA(&UART_HANDLE, uartRequestBuffer, UART_DATA_BUFFER_SIZE);
}

void appLoop() {
	// We recieved something - let's check what exactly happened
	if (uartRequestLength) {
		printf("Received %d bytes, so we should have %d-byte long protobuf\r\n",
				uartRequestLength, uartRequestLength - 2);

		// Handle the message
		// First, let's extract the protobuf length
		uint16_t const requestLength = getSizeHeader(uartRequestBuffer);
		printf("Extracted length: %d\r\n", requestLength);
		// This function accepts only protobuf data, so we have to offset the data pointer by 2 header bytes
		// We'll give this function an address to output buffer with 2-byte offset, so it'll be easy for us to fill it later with response size
		uint16_t const responseLength = (uint16_t) handleMessage(
				uartRequestBuffer + HEADER_SIZE_LENGTH, requestLength,
				uartResponseBuffer + HEADER_SIZE_LENGTH, UART_DATA_BUFFER_SIZE);
		// we assume that the returned message is only Protobuf data, so we have to add our header
		setSizeHeader(uartResponseBuffer, responseLength);
		// And off we go with the packet - don't forget to add length field size here!
		HAL_UART_Transmit(&UART_HANDLE, uartResponseBuffer,
				responseLength + HEADER_SIZE_LENGTH, HAL_MAX_DELAY);
		printf("Responded with %d bytes (excluding length)\r\n",
				responseLength);

		// Reset transaction and clear the request length
		HAL_UART_Abort(&UART_HANDLE);
		HAL_UARTEx_ReceiveToIdle_DMA(&UART_HANDLE, uartRequestBuffer,
		UART_DATA_BUFFER_SIZE);
		uartRequestLength = 0;
	}

	// If data output is enabled, we generate and transmit the data here every 100ms (by default)
	if (getAppState() == APP_STATE_TRANSMITTING_DATA) {
		if (HAL_GetTick() - lastDataSentTimestamp > dataDelay) {
			// Let's generate some data
			uint16_t const responseLength = (uint16_t) generateData(
					uartResponseBuffer + HEADER_SIZE_LENGTH,
					UART_DATA_BUFFER_SIZE);
			// Add it's length to packet
			setSizeHeader(uartResponseBuffer, responseLength);
			// And send it out
			HAL_UART_Transmit(&UART_HANDLE, uartResponseBuffer,
					responseLength + HEADER_SIZE_LENGTH, HAL_MAX_DELAY);
			// also make sure to increment packet number
			incrementPacketNumber();
			// and update timestamp
			lastDataSentTimestamp = HAL_GetTick();

			printf("Sent data, %d bytes (excluding length)\r\n",
					responseLength);
		}
	}
}

void setSizeHeader(uint8_t* buffer, uint16_t size) {
	// We'll use my helper macro from bit_helpers.h
	// Size will be encoded in little endian format (LSB first in memory)
	VALUE_TO_16BIT_BYTEARRAY_LE(size, buffer);
}

uint16_t getSizeHeader(uint8_t* buffer) {
	// We'll use my helper macro from bit_helpers.h
	// Size will be decoded in little endian format (LSB first in memory)
	return BYTEARRAY_TO_16BIT_VALUE_LE(buffer);
}

// stdout redirected to debugger's ITM, so we can have debug prints
// without interfering with communication
int __io_putchar(int ch) {
	return ITM_SendChar(ch);
}

// Idle line event handler
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
	uartRequestLength = Size;
}
