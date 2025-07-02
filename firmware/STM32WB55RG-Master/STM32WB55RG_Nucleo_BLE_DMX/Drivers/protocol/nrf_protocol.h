/**
 * @file nrf_protocol.h
 * @brief Header file for NRF24L01 protocol handling for DMX512 data transmission.
 * @author Guillermo Morancho
 * @date June 12, 2025
 */

#ifndef PROTOCOL_NRF_PROTOCOL_H_
#define PROTOCOL_NRF_PROTOCOL_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "nrf24l01.h"

 /**
  * @brief Maximum size of a single NRF24L01 packet (32 bytes).
  */
#define NRF24_PACKET_SIZE	32

 /**
  * @brief Sends BLE data via NRF24L01.
  * @param txAddr Pointer to the 5-byte TX address.
  * @param data Pointer to the data buffer.
  * @param len Length of the data buffer.
  * @return NR24Res_TypeDef Result of the operation.
  */
 NR24Res_TypeDef SendBLEDataViaNRF24(uint8_t *txAddr, uint8_t *data, uint8_t len);

 /**
  * @brief Sends synchronization messages (CMD_SYNC) via NRF24L01.
  * @param txAddr Pointer to the 5-byte TX address.
  * @param dmx_buffer Pointer to the DMX buffer (Start Code + 512 channels).
  * @return NR24Res_TypeDef Result of the operation.
  */
 NR24Res_TypeDef SendSyncMessages(uint8_t *txAddr, const uint8_t* dmx_buffer);

 #ifdef __cplusplus
 }
 #endif

 #endif /* PROTOCOL_NRF_PROTOCOL_H_ */
