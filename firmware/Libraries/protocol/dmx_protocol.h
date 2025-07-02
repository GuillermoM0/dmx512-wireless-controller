/**
 * @file dmx_protocol.h
 * @brief DMX protocol handler for parsing control commands and updating DMX channels.
 * @author Guillermo Morancho
 * @date June 10, 2025
 */
#ifndef DMX512_DMX_PROTOCOL_H_
#define DMX512_DMX_PROTOCOL_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

#define CMD_CONTROL				0x00	/**< Control command (e.g., start/stop transmission) */
#define CMD_SET_MULTIPLE_VALUES	0x01	/**< Set multiple channels with values */
#define CMD_SET_CHANNELS		0x02	/**< Set specific channels to a single value */
#define CMD_SYNC				0x10	/**< Synchronize DMX buffer with a packet */
#define CMD_RESET				0xff	/**< Reset DMX buffer to default values */

 /**
  * @brief Result codes for DMX data parsing
  */
 typedef enum {
     PARSE_SUCCESS = 0,          /**< Parsing completed successfully */
     PARSE_ERROR = 1,            /**< General parsing error */
     PARSE_INVALID_COMMAND,      /**< Unknown command received */
     PARSE_BUFFER_OVERFLOW,      /**< Data exceeds expected length */
     PARSE_INVALID_CHANNEL       /**< Invalid channel number in data */
 } ParseResult;

/**
  * @brief Parses incoming DMX data and executes corresponding commands.
  * @param data Pointer to the incoming data buffer.
  * @param length Length of the data buffer.
  * @return ParseResult Result of the parsing operation.
  */
 ParseResult ParseDMXData(const uint8_t* data, uint8_t length);

#endif /* DMX512_DMX_PROTOCOL_H_ */
