/**
 * @file dmx_protocol.c
 * @brief Implementation of DMX protocol parsing functions.
 * @author Guillermo Morancho
 * @date June 10, 2025
 */
#include "dmx_protocol.h"
#include "dmx512.h"

/**
 * @brief Parses incoming DMX data and executes corresponding commands.
 * @param data Pointer to the incoming data buffer.
 * @param length Length of the data buffer.
 * @return ParseResult Result of the parsing operation.
 */
ParseResult ParseDMXData(const uint8_t* data, uint8_t length) {
    if (length < 1 || data == NULL) {
        return PARSE_ERROR;  // Invalid input
    }

    uint8_t cmdType = data[0];

    switch (cmdType) {
        case CMD_CONTROL:
            {
                if (length < 2) {
                    return PARSE_BUFFER_OVERFLOW;  // Not enough data for CMD_CONTROL
                }

                if (data[1] == 0) {
                    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
                    DMX512_StopTask();
                } else if (data[1] == 1) {
                    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
                    DMX512_StartTask();
                } else {
                    return PARSE_INVALID_COMMAND;  // Invalid control value
                }
            }
            break;

        case CMD_SET_MULTIPLE_VALUES:
            {
                if (length < 2) {
                    return PARSE_BUFFER_OVERFLOW;  // Not enough data for CMD_SET_MULTIPLE_VALUES
                }

                uint8_t channelCount = data[1];
                if (length < 2 + (channelCount * 3)) {
                    return PARSE_BUFFER_OVERFLOW;  // Not enough data for all channels
                }

                for (uint8_t i = 0; i < channelCount; i++) {
                    uint8_t offset = 2 + i * 3;
                    uint8_t value = data[offset];
                    uint16_t channel = (data[offset + 1] << 8) | data[offset + 2];

                    if (channel >= 1 && channel <= 512) {
                        DMX512_SetChannel(channel, value);
                    } else {
                        return PARSE_INVALID_CHANNEL;  // Invalid channel number
                    }
                }
            }
            break;

        case CMD_SET_CHANNELS:
            {
                if (length < 2) {
                    return PARSE_BUFFER_OVERFLOW;  // Not enough data for CMD_SET_CHANNELS
                }

                uint8_t channelCount = data[1];
                uint8_t value = data[2];

                if (length < 2 + (channelCount * 2)) {
                    return PARSE_BUFFER_OVERFLOW;  // Not enough data for all channels
                }

                for (uint8_t i = 0; i < channelCount; i++) {
                    uint16_t channel = (data[3 + (i * 2)] << 8) | data[3 + (i * 2) + 1];

                    if (channel >= 1 && channel <= 512) {
                        DMX512_SetChannel(channel, value);
                    } else {
                        return PARSE_INVALID_CHANNEL;  // Invalid channel number
                    }
                }
            }
            break;

        case CMD_SYNC:
            {
                uint8_t* dmx_buf = (uint8_t*)DMX512_GetBuffer();
                if (length < 2) {
                    return PARSE_BUFFER_OVERFLOW;  // Not enough data for CMD_SYNC
                }

                uint8_t packet_num = data[1];
                uint16_t start_channel = packet_num * 30 + 1;

                uint8_t values_in_packet = length - 2;
                if (start_channel + values_in_packet - 1 > 512) {
                    values_in_packet = 512 - start_channel + 1;
                }

                for (uint8_t i = 0; i < values_in_packet; i++) {
                    dmx_buf[start_channel + i] = data[2 + i];
                }
            }
            break;

        case CMD_RESET:
            {
                DMX512_ResetBuffer();
            }
            break;

        default:
            return PARSE_INVALID_COMMAND;  // Unknown command
    }

    return PARSE_SUCCESS;  // Parsing completed successfully
}
