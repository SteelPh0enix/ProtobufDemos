/*
 * data_generator.c
 *
 *  Created on: Dec 15, 2021
 *      Author: steelph0enix
 */

#include "app/data_generator.h"
#include "app/app_state.h"
#include "app/utils.h"

#include <pb_encode.h>
#include "ExampleProto.pb.h"

#include <stdlib.h>
#include <stdio.h>

size_t generateData(uint8_t* dataBuffer, size_t bufferSize) {
	// Create empty, zero'd message
	DataPacket packet = DataPacket_init_zero;

	// Create output stream
	pb_ostream_t stream = pb_ostream_from_buffer(dataBuffer, bufferSize);

	// Fill the message with data
	packet.valueA = generateRandomDouble(0, 100, 2);
	packet.valueB = generateRandomDouble(0, 1000, 2);
	packet.valueC = generateRandomDouble(100, 10000, 3);
	packet.constantValue = getConstantValue();
	packet.packetNumber = getPacketNumber();

	// Encode the message into the buffer
	if (!pb_encode(&stream, DataPacket_fields, &packet)) {
		printf("Couldn't encode data packet, reason: %s\r\n", PB_GET_ERROR(&stream));
	}

	// And let's return the amount of bytes written
	return stream.bytes_written;
}
