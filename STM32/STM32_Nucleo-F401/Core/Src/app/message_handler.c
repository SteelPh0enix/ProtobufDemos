/*
 * message_handler.c
 *
 *  Created on: Dec 15, 2021
 *      Author: steelph0enix
 */

#include "app/message_handler.h"
#include "app/app_state.h"
#include "main.h"

#include <pb_decode.h>
#include <pb_encode.h>
#include "ExampleProto.pb.h"

#include <stdio.h>
#include <stdbool.h>

size_t generateResponse(DeviceResponse_ResponseType responseCode,
		uint8_t* responseBuffer, size_t responseBufferSize);
bool handleRequest(DeviceRequest* const request);

size_t handleMessage(uint8_t const* message, size_t messageSize,
		uint8_t* responseBuffer, size_t responseBufferSize) {
	DeviceRequest request = DeviceRequest_init_zero;

	// Let's create an input and output stream for protobuf
	pb_istream_t request_stream = pb_istream_from_buffer(message, messageSize);

	// Let's try to decode the incoming message...
	if (!pb_decode(&request_stream, DeviceRequest_fields, &request)) {
		printf("Couldn't decode the request, reason: %s\r\n",
				PB_GET_ERROR(&request_stream));
		// If that happens, we're gonna return response with PROTOBUF_ERROR type.
		return generateResponse(DeviceResponse_ResponseType_PROTOBUF_ERROR,
				responseBuffer, responseBufferSize);
	}

	// Right now, we should have the request data put into `request` structure instance
	// handleRequest will return `true= on success, and `false` on wrong request type
	if (handleRequest(&request)) {
		return generateResponse(DeviceResponse_ResponseType_OK, responseBuffer,
				responseBufferSize);
	}

	return generateResponse(DeviceResponse_ResponseType_BAD_TYPE,
			responseBuffer, responseBufferSize);
}

size_t generateResponse(DeviceResponse_ResponseType responseCode,
		uint8_t* responseBuffer, size_t responseBufferSize) {
	DeviceResponse response = DeviceResponse_init_zero;
	pb_ostream_t response_stream = pb_ostream_from_buffer(responseBuffer,
			responseBufferSize);

	response.code = responseCode;

	if (!pb_encode(&response_stream, DeviceResponse_fields, &response)) {
		printf("Couldn't encode the response, reason: %s\r\n",
				PB_GET_ERROR(&response_stream));
	}

	return response_stream.bytes_written;
}

bool handleRequest(DeviceRequest* const request) {
	switch (request->code) {
	case DeviceRequest_RequestType_CHANGE_LED_STATE:
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		break;
	case DeviceRequest_RequestType_START:
		setAppState(APP_STATE_TRANSMITTING_DATA);
		break;
	case DeviceRequest_RequestType_STOP:
		setAppState(APP_STATE_IDLE);
		break;
	default:
		return false;
	}
	return true;
}
