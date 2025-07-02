/**
 * @file dmx512.c
 * @brief Implementation of the DMX512 signal generator library.
 * @author Guillermo Morancho
 * @date May 16, 2025
*/

#include "dmx512.h"
#include <string.h>
#include "led.h"

// DMX Buffer and Configuration
static UART_HandleTypeDef *dmx_huart;				// UART han for DMX transmissiondle
static TIM_HandleTypeDef  *dmx_htim;				// Timer handle for microsecond delays
static uint8_t dmx_buffer[DMX_BUFFER_SIZE] = {0};	// DMX buffer (Start Code + 512 channels)

// State flags
static volatile uint8_t dma_transfer_complete = 1;	// Flag for DMA completion
static volatile uint8_t delay_complete = 0;			// Flag for microsecond delay completion
static DMX_State dmx_state = DMX_IDLE;				// Current state in the state machine
static uint32_t last_frame_time = 0;				// Timestamp of last frame transmission
static uint8_t DMX_Stop = 1;
/**
 * @brief Generate a non-blocking microsecond delay using a timer
 * @param us Delay duration in microseconds
*/
static void delay_us(uint32_t us) {
    /* Asegurarse de que no esté ya corriendo */
    HAL_TIM_Base_Stop_IT(dmx_htim);

    delay_complete = 0;
    /* Fijar auto-reload al número de microsegundos deseado */
    __HAL_TIM_SET_AUTORELOAD(dmx_htim, us);

    /* Reiniciar contador */
    __HAL_TIM_SET_COUNTER(dmx_htim, 0);

    /* Arrancar timer en modo interrupción */
    HAL_TIM_Base_Start_IT(dmx_htim);
}

//static void delay_us_blok(uint16_t us) {
//    __HAL_TIM_SET_COUNTER(dmx_htim, 0);
//    HAL_TIM_Base_Start(dmx_htim);
//    while (__HAL_TIM_GET_COUNTER(dmx_htim) < us);
//    HAL_TIM_Base_Stop(dmx_htim);
//}

/**
 * @brief Configure TX pin as GPIO output for Break/MAB generation
*/
static void DMX512_Tx_Output(void){
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = DMX_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(DMX_TX_GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief Reconfigure TX pin to alternate function for UART transmission
*/
static void DMX512_Tx_Alter_Function(void){
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = DMX_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = DMX_UART_AF;
    HAL_GPIO_Init(DMX_TX_GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief Initialize DMX512 library
 * @param huart Pointer to USART handle
 * @param htim  Pointer to timer handle
 * @return HAL_StatusTypeDef
*/
HAL_StatusTypeDef DMX512_Init(UART_HandleTypeDef *huart, TIM_HandleTypeDef *htim) {
	dmx_huart = huart;
	dmx_htim  = htim;

    // Configure UART for DMX (250 kbps, 8N2)
    huart->Init.BaudRate = 250000;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.StopBits = UART_STOPBITS_2;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    if (HAL_UART_Init(huart) != HAL_OK) return HAL_ERROR;

    // Configure Timer for 1 µs ticks
    htim->Init.Prescaler = 32 - 1; //Timer MHz - 1
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim->Init.Period = 0xFFFFFFFF;
    htim->Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(htim) != HAL_OK) return HAL_ERROR;

    if (HAL_TIM_Base_Start_IT(htim) != HAL_OK) return HAL_ERROR;

    DMX_Stop = 0;
    return HAL_OK;
}

/**
 * @brief Set a DMX channel value
 * @param channel Channel number (1-512)
 * @param value   0-255
*/
void DMX512_SetChannel(uint16_t channel, uint8_t value) {
	if (channel > 0 && channel <= DMX_MAX_CHANNELS) {
		dmx_buffer[channel] = value;
	}
}

/**
 * @brief Set the DMX Start Code
 * @param code Start code (0x00 for standard DMX)
*/
void DMX512_SetStartCode(uint8_t code) {
    dmx_buffer[0] = code;
}

/**
 * @brief Set the entire DMX channel buffer (excluding start code).
 *
 * @param data Pointer to an array of 512 bytes containing channel data.
 */
void DMX512_SetBuffer(const uint8_t *data) {
    if (data == NULL) return;

    // Copy 512 channel values (excluding start code at index 0)
    memcpy(&dmx_buffer[1], data, DMX_MAX_CHANNELS);
}

/**
 * @brief Blocking DMX frame send function (not recommended for real-time systems)
*/
void DMX512_SendFrame(void) {
	while (!dma_transfer_complete);

    DMX_ENABLE_TX();

    // 1. Generate Break (88–104 µs)
	DMX512_Tx_Output();
	HAL_GPIO_WritePin(DMX_TX_GPIO_Port, DMX_TX_Pin, GPIO_PIN_RESET);  // LOW
	delay_us(DMX_BREAK_US);
	while (!delay_complete);

    // 2. Generate MAB (12 µs)
	HAL_GPIO_WritePin(DMX_TX_GPIO_Port, DMX_TX_Pin, GPIO_PIN_SET);  // HIGH
	DMX512_Tx_Alter_Function();
	delay_us(DMX_MAB_US);
	while (!delay_complete);

    // 3. Transmit data via DMA
	dma_transfer_complete = 0;
	HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(dmx_huart, dmx_buffer, DMX_BUFFER_SIZE);
	if (status != HAL_OK) {
	    Error_Handler();
	}
}

/**
 * @brief Non-blocking task to manage DMX frame transmission
*/
void DMX512_Task(void) {
	switch (dmx_state) {
		case DMX_IDLE:
            // Wait for frame interval (22.8 ms)
			if (!DMX_Stop){
				uint32_t current_time = HAL_GetTick();
				if ((current_time - last_frame_time) >= DMX_FRAME_INTERVAL_MS) {
					last_frame_time = current_time;

			        HAL_GPIO_WritePin(LED4_GPIO_PORT, LED4_PIN, GPIO_PIN_SET);
					// Transition to BREAK state
					dmx_state = DMX_BREAK;
					DMX_ENABLE_TX(); // Enable MAX485 transmitter
					DMX512_Tx_Output();
					HAL_GPIO_WritePin(DMX_TX_GPIO_Port, DMX_TX_Pin, GPIO_PIN_RESET);  // LOW
					delay_us(DMX_BREAK_US);  // Start non-blocking delay
				}
			}
			break;

		case DMX_BREAK:
            // Wait for BREAK
			if (delay_complete) {
				// Transition to MAB state
				dmx_state = DMX_MAB;
				HAL_GPIO_WritePin(DMX_TX_GPIO_Port, DMX_TX_Pin, GPIO_PIN_SET);  // HIGH
				delay_us(DMX_MAB_US);  // Start non-blocking delay
				DMX512_Tx_Alter_Function();
			}
			break;

		case DMX_MAB:
            // Wait for MAB
			if (delay_complete) {
	            // Start DMA transmission
				dmx_state = DMX_TRANSMIT_DATA;
				HAL_UART_Transmit_DMA(dmx_huart, dmx_buffer, DMX_BUFFER_SIZE);
			}
			break;

		case DMX_TRANSMIT_DATA:
			// Wait for DMA to complete
			if (dma_transfer_complete) {
				dmx_state = DMX_IDLE;
			}
			break;
	}
}

/**
 * @brief Returns the current state of the DMX state machine.
 *
 * @return DMX_State Current state (e.g., DMX_IDLE, DMX_BREAK, etc.).
*/
DMX_State DMX512_GetState(void) {
	return dmx_state;
}

/**
 * @brief Checks if a DMX transmission is currently active.
 *
 * @return uint8_t 1 if transmitting, 0 if not.
*/
uint8_t DMX512_IsTransmitting(void) {
	return (!DMX_Stop);
}

/**
 * @brief Starts DMX transmission.
*/
void DMX512_StartTask(void) {
	DMX_Stop = 0;
}

/**
 * @brief Stops DMX transmission.
*/
void DMX512_StopTask(void) {
	DMX_Stop = 1;
}

/**
 * @brief Reset the DMX buffer to 0 (Start Code + 512 channels).
*/
void DMX512_ResetBuffer(void) {
    memset(dmx_buffer, 0, DMX_BUFFER_SIZE);
}

/**
 * @brief Retrieves the value of a specific DMX channel.
 *
 * @param channel Channel number (1-512).
 * @return uint8_t Channel value (0-255). Returns 0 for invalid channels.
*/
uint8_t DMX512_GetChannelValue(uint16_t channel) {
	if (channel > 0 && channel <= DMX_MAX_CHANNELS) {
		return dmx_buffer[channel];
	}
	return 0; // Return 0 for invalid channel indices
}

/**
 * @brief Retrieves the DMX Start Code (first byte of the DMX buffer).
 *
 * @return uint8_t Start Code value (0x00 for standard DMX).
*/
uint8_t DMX512_GetStartCode(void) {
	return dmx_buffer[0];
}

/**
 * @brief Provides a read-only pointer to the entire DMX buffer.
 *
 * @warning Do not modify the buffer directly; use DMX512_SetChannel() instead.
 * @return const uint8_t* Pointer to the DMX buffer (Start Code + 512 channels).
*/
const uint8_t* DMX512_GetBuffer(void) {
	return dmx_buffer;
}

/**
 * @brief Timer interrupt callback to mark microsecond delay completion
*/
void DMX_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if (htim == dmx_htim) {
        delay_complete = 1; // Mark delay as complete
        HAL_TIM_Base_Stop_IT(htim); // Stop the timer
        __HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE); // Clear the interrupt flag
    }
}

/**
 * @brief UART transmission complete callback
*/
void DMX_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == dmx_huart) {
        dma_transfer_complete = 1;  // Mark DMA as complete
        HAL_GPIO_WritePin(LED4_GPIO_PORT, LED4_PIN, GPIO_PIN_RESET);
		//DMX_DISABLE_TX();  // Return to receive mode
    }
}
