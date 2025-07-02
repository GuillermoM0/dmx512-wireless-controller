/*
 * dmx_protocol.h
 *
 *  Created on: Jun 10, 2025
 *      Author: User
 */

#ifndef DMX512_DMX_PROTOCOL_H_
#define DMX512_DMX_PROTOCOL_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

#define CMD_CONTROL				0x00
#define CMD_SET_MULTIPLE_VALUES	0x01
#define CMD_SET_CHANNELS		0x02
#define CMD_SYNC				0x10
#define CMD_RESET				0xff

 uint8_t ParseDMXData(const uint8_t* data, uint8_t length);

#endif /* DMX512_DMX_PROTOCOL_H_ */
