/*
 * nrf_protocol.h
 *
 *  Created on: Jun 12, 2025
 *      Author: User
 */

#ifndef PROTOCOL_NRF_PROTOCOL_H_
#define PROTOCOL_NRF_PROTOCOL_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "nrf24l01.h"

#define NRF24_PACKET_SIZE	32

 NR24Res_TypeDef SendBLEDataViaNRF24(uint8_t *txAddr, uint8_t *data, uint8_t len);

#endif /* PROTOCOL_NRF_PROTOCOL_H_ */
