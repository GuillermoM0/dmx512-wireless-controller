/******************************************************************************
 * @file	app.c
 * @author  UdL
 * @version V1.0.0
 * @date    1-Jan-2021
 * @brief   This file contains the body of the main application.
 ******************************************************************************
 * @attention
 *
 * <h2>&copy; Copyright (c) 2022 UDL. All rights reserved.</h2>
 *
 ******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "app.h"
#include "main.h"
#include "led.h"
#include "pushbutton.h"
#include "uart_debug.h"
#include "stdio.h"    	// for printf()/sprintf()
#include "stringcmd.h"  // string user functions
#include "string.h"
#include "stdlib.h"
#include "dmx512.h"
#include "nrf24l01.h"
#include "dmx_protocol.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
uint8_t i = 0;
uint8_t rx_address[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
uint8_t rx_data[NRF24_PLOAD_WIDTH];
uint8_t rx_len;

uint32_t NRF24_DataReceived_time = 0;

// Main application Initialization
void APP_Init(void) {
	// Printf redirected to USART2 send data
	printf("Init Ok! DMX-512 example ...\r");

	NRF24_EnablePipeRX(NRF24_PIPE0, rx_address);
	NRF24_Mode(NRF24_MODE_RX);
}

// Application Task
void APP_Task(void) {
	#ifndef UART_RX_INTERRUPT
	  // Polling mode
	  UART_task();
	#endif

	// Check if an string is received
	if (UART_isDataRecived() == SET) {
		char cmd[100];
		// Gets the new string and release the flag for new read
		UART_GetStr(cmd);

		// Do ECHO
		printf("Command received: --%s--\r", cmd);


		// USART Commands
		/////////////////

		// Send data through  RS-485 -> 'S "data to send"'
		if (cmd[0]=='S' && cmd[1]==' ') {

			printf(")\r");

		// No valid command
		} else {
			printf("NV\r");
		}
	}

	if (HAL_GPIO_ReadPin(LED5_GPIO_PORT, LED5_PIN) && (HAL_GetTick()-NRF24_DataReceived_time) > 100) {
		HAL_GPIO_WritePin(LED5_GPIO_PORT, LED5_PIN, GPIO_PIN_RESET);
	}

#ifndef NRF24_RX_IRQ_MODE
	NRF24_CheckDataRX();
#endif

    if (NRF24_isRecivData(NRF24_PIPE0)) {
        if (NRF24_ReadData(NRF24_PIPE0, rx_data, &rx_len) == NRF24_SUCCESS) {
        	ParseDMXData(rx_data, rx_len);
        	HAL_GPIO_WritePin(LED5_GPIO_PORT, LED5_PIN, GPIO_PIN_SET);
        	NRF24_DataReceived_time = HAL_GetTick();
        }
    }
//	DMX512_SendFrame();
//	HAL_Delay(50);
	DMX512_Task();

}


/*************************************************************************
 * INTERRUPT CALLBACKS                                                   *
 *************************************************************************/

/**
  * @brief EXTI line detection callback.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch(GPIO_Pin) {
    	case BUTTON_USER_PIN:
    		switch (i) {
    			case 0:
    			i = 1;
    			DMX512_SetChannel(1, 255);
    			DMX512_SetChannel(2, 0);
    			DMX512_SetChannel(3, 0);
    			break;

    			case 1:
    			i = 2;
    			DMX512_SetChannel(1, 0);
    			DMX512_SetChannel(2, 255);
    			DMX512_SetChannel(3, 0);
    			break;

    			case 2:
    			i = 0;
    			DMX512_SetChannel(1, 0);
    			DMX512_SetChannel(2, 0);
    			DMX512_SetChannel(3, 255);
    			break;
    		}

    		break;

		case NRF24_IRQ_Pin:
			NRF24_IRQ_Callback();
			break;

    	default:
    		break;
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	DMX_TIM_PeriodElapsedCallback(htim);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	DMX_UART_TxCpltCallback(huart);
}


