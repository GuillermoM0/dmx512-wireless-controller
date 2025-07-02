/**
 * @file nrf_protocol.c
 * @brief Implementation of NRF24L01 protocol handling for DMX512 data transmission.
 * @author Guillermo Morancho
 * @date June 12, 2025
 */

#include "nrf_protocol.h"
#include "dmx_protocol.h"

/**
 * @brief Sends BLE data via NRF24L01 module.
 * Handles both single-packet and multi-packet transmission based on command type.
 * @param txAddr Pointer to the 5-byte TX address (e.g., "DMX01").
 * @param data Pointer to the data buffer to send.
 * @param len Length of the data buffer.
 * @return NR24Res_TypeDef Result of the transmission operation.
 */
NR24Res_TypeDef SendBLEDataViaNRF24(uint8_t *txAddr, uint8_t *data, uint8_t len) {
    if (len <= NRF24_PACKET_SIZE) {
        // Single-packet transmission
        NR24Res_TypeDef result = NRF24_SendData(txAddr, data, len);
        return result;
    } else {
        // Multi-packet transmission for large data
        uint8_t cmdType = data[0];

        switch (cmdType) {
            case CMD_SET_MULTIPLE_VALUES:
                {
                    // Validate command structure
                    if (len < 2) return NRF24_ERROR;
                    uint8_t channelCount = data[1];
                    if (len < 2 + (channelCount * 3)) return NRF24_ERROR;

                    // Split into 10-channel packets
                    uint8_t packets = (channelCount + 9) / 10;
                    uint8_t nrfPacket[NRF24_PACKET_SIZE];

                    for (uint8_t packet = 0; packet < packets; packet++) {
                        uint8_t channelsInPacket = 10;
                        if (packet == packets - 1 && channelCount % 10 != 0) {
                            channelsInPacket = channelCount % 10;
                        }

                        // Build packet
                        nrfPacket[0] = CMD_SET_MULTIPLE_VALUES;
                        nrfPacket[1] = channelsInPacket;
                        uint16_t startOffset = 2 + (packet * 30);
                        memcpy(&nrfPacket[2], &data[startOffset], channelsInPacket * 3);

                        // Send packet
                        uint8_t payloadLength = 2 + (channelsInPacket * 3);
                        NR24Res_TypeDef result = NRF24_SendData(txAddr, nrfPacket, payloadLength);

                        if (result != NRF24_SUCCESS) {
                            return result;
                        }
                    }
                    return NRF24_SUCCESS;
                }
                break;

            case CMD_SET_CHANNELS:
                {
                    // Validate command structure
                    if (len < 2) return NRF24_ERROR;
                    uint8_t channelCount = data[1];
                    uint8_t value = data[2];
                    if (len < 2 + (channelCount * 2)) return NRF24_ERROR;

                    // Split into 14-channel packets
                    uint8_t channelsPerPacket = 14;
                    uint8_t packets = (channelCount + channelsPerPacket - 1) / channelsPerPacket;
                    uint8_t nrfPacket[NRF24_PACKET_SIZE];

                    for (uint8_t packet = 0; packet < packets; packet++) {
                        uint8_t channelsInPacket = (packet == packets - 1) ? (channelCount % channelsPerPacket) : channelsPerPacket;
                        if (channelsInPacket == 0) channelsInPacket = channelsPerPacket;

                        // Build packet
                        nrfPacket[0] = CMD_SET_CHANNELS;
                        nrfPacket[1] = channelsInPacket;
                        nrfPacket[2] = value;
                        uint16_t startOffset = 3 + (packet * channelsPerPacket * 2);
                        memcpy(&nrfPacket[3], &data[startOffset], channelsInPacket * 2);

                        // Send packet
                        uint8_t payloadLength = 3 + (channelsInPacket * 2);
                        NR24Res_TypeDef result = NRF24_SendData(txAddr, nrfPacket, payloadLength);

                        if (result != NRF24_SUCCESS) {
                            return result;
                        }
                    }
                    return NRF24_SUCCESS;
                }
                break;

            default:
                // Unknown command
                return NRF24_ERROR;
        }
    }
}

/**
 * @brief Sends synchronization messages (e.g., DMX buffer) via NRF24L01.
 * Splits the DMX buffer into 18 packets (30 channels per packet).
 * @param txAddr Pointer to the 5-byte TX address (e.g., "DMX01").
 * @param dmx_buffer Pointer to the DMX buffer (Start Code + 512 channels).
 * @return NR24Res_TypeDef Result of the transmission operation.
 */
NR24Res_TypeDef SendSyncMessages(uint8_t *txAddr, const uint8_t* dmx_buffer) {
    uint8_t sync_data[NRF24_PACKET_SIZE];
    sync_data[0] = CMD_SYNC;  // Command ID

    for (uint8_t packet = 0; packet < 18; packet++) {
        sync_data[1] = packet;  // Packet number

        uint16_t start_idx = (packet * 30) + 1;
        uint8_t channels_to_send = (packet == 17) ? 2 : 30;

        memcpy(&sync_data[2], &dmx_buffer[start_idx], channels_to_send);

        NR24Res_TypeDef result = NRF24_SendData(txAddr, sync_data, 2 + channels_to_send);

        if (result != NRF24_SUCCESS) {
            return result;
        }
    }
    return NRF24_SUCCESS;
}
