/*
 * message_handler.h
 *
 *  Created on: Dec 15, 2021
 *      Author: steelph0enix
 */

#ifndef INC_NANOPB_MESSAGE_HANDLER_H_
#define INC_NANOPB_MESSAGE_HANDLER_H_

#include <stdint.h>
#include <stddef.h>

size_t handleMessage(uint8_t const* message, size_t messageSize, uint8_t* responseBuffer, size_t responseBufferSize);

#endif /* INC_NANOPB_MESSAGE_HANDLER_H_ */
