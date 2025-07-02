/*
 * dmx_protocol.c
 *
 *  Created on: Jun 10, 2025
 *      Author: User
 */
#include "dmx_protocol.h"
#include "dmx512.h"
#include "led.h"

uint8_t ParseDMXData(const uint8_t* data, uint8_t length) {
    if (length < 1 || data == NULL) return 0;

	uint8_t cmdType = data[0];

	switch (cmdType) {
		case CMD_CONTROL:
			{
				if (length < 2) return 0;
				if (data[1] == 0) {
//					HAL_GPIO_WritePin(LED4_GPIO_PORT, LED4_PIN, GPIO_PIN_RESET);
					DMX512_StopTask();
				} else if (data[1] == 1) {
//					HAL_GPIO_WritePin(LED4_GPIO_PORT, LED4_PIN, GPIO_PIN_SET);
					DMX512_StartTask();
				}
			}
				break;

		case CMD_SET_MULTIPLE_VALUES:
			{
				if (length < 2) return 0;
				uint8_t channelCount = data[1];
				if (length < 2 + (channelCount * 3)) return 0;

				for (uint8_t i = 0; i < channelCount; i++) {
					uint8_t offset = 2 + i * 3;
					uint8_t value = data[offset];
					uint16_t channel = (data[offset + 1] << 8) | data[offset + 2];
					if (channel >= 1 && channel <= 512) {
						DMX512_SetChannel(channel, value);
					}
				}
			}
			break;

		case CMD_SET_CHANNELS:
			{
				if (length < 2) return 0;
				uint8_t channelCount = data[1];
				uint8_t value = data[2];

				if (length < 2 + (channelCount * 2)) return 0;

				for (uint8_t i = 0; i < channelCount; i++) {
					uint16_t channel = (data[3 + (i*2)] << 8) | data[3 + (i*2) + 1];
					if (channel >= 1 && channel <= 512) {
						DMX512_SetChannel(channel, value);
					}
				}
			}
			break;

		case CMD_SYNC:
			{
				uint8_t* dmx_buf = (uint8_t*)DMX512_GetBuffer();
				if (length < 2) return 0;
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
			// Comando desconocido
			return 0;
	}
	return 1;
}
