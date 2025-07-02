/*
 * nrf_protocol.c
 *
 *  Created on: Jun 12, 2025
 *      Author: User
 */

#include "nrf_protocol.h"
#include "dmx_protocol.h"

NR24Res_TypeDef SendBLEDataViaNRF24(uint8_t *txAddr, uint8_t *data, uint8_t len){
	if (len <= NRF24_PACKET_SIZE){
		NR24Res_TypeDef result = NRF24_SendData(txAddr, data, len);
		return result;

	} else {
		uint8_t cmdType = data[0];

		switch (cmdType) {
			case CMD_SET_MULTIPLE_VALUES:
				{
					if (len < 2) return 2;
					uint8_t channelCount = data[1];
					if (len < 2 + (channelCount * 3)) return 2;

					uint8_t packets = (channelCount + 9) / 10;
					uint8_t nrfPacket[NRF24_PACKET_SIZE];

					for (uint8_t packet = 0; packet < packets; packet++) {
						uint8_t channelsInPacket = 10;
						if (packet == packets - 1 && channelCount % 10 != 0) {
							channelsInPacket = channelCount % 10;
						}

						nrfPacket[0] = CMD_SET_MULTIPLE_VALUES;
						nrfPacket[1] = channelsInPacket;

						uint16_t startOffset = 2 + (packet * 30);
						memcpy(&nrfPacket[2], &data[startOffset], channelsInPacket * 3);

						uint8_t payloadLength = 2 + (channelsInPacket * 3);
						NR24Res_TypeDef result = NRF24_SendData(txAddr, nrfPacket, payloadLength);

						// Manejar éxito en el envío
						if (result != NRF24_SUCCESS) {
							return result;
						}
					}
					return NRF24_SUCCESS;
				}
				break;

			case CMD_SET_CHANNELS:
				{
					if (len < 2) return 0;
					uint8_t channelCount = data[1];
					uint8_t value = data[2];
					if (len < 2 + (channelCount * 2)) return 0;

					uint8_t channelsPerPacket = 14;
					uint8_t packets = (channelCount + channelsPerPacket - 1) / channelsPerPacket;
					uint8_t nrfPacket[NRF24_PACKET_SIZE];

					for (uint8_t packet = 0; packet < packets; packet++) {
						uint8_t channelsInPacket = (packet == packets - 1) ? (channelCount % channelsPerPacket) : channelsPerPacket;
						if (channelsInPacket == 0) channelsInPacket = channelsPerPacket;  // En caso de división exacta

						nrfPacket[0] = CMD_SET_CHANNELS;
						nrfPacket[1] = channelsInPacket;
						nrfPacket[2] = value;

						uint16_t startOffset = 3 + (packet * channelsPerPacket * 2);  // Inicio de los canales en el buffer BLE
						memcpy(&nrfPacket[3], &data[startOffset], channelsInPacket * 2);

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
				// Comando desconocido
				return 2;
		}
		return 2;
	}
}
