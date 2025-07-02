/**
 * @file dmx512.h
 * @brief DMX512 signal generator library for STM32 using DMA and Timer-driven approach.
 * @author Guillermo Morancho
 * @date May 16, 2025
*/

#ifndef DMX512_H_
#define DMX512_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

/**
 * @defgroup DMX_Protocol_Parameters DMX Protocol Parameters
 * @{
*/
#define DMX_MAX_CHANNELS		512						// Maximum number of DMX channels
#define DMX_BUFFER_SIZE			(DMX_MAX_CHANNELS + 1)	// Buffer size includes Start Code (1 byte) + 512 channels
#define DMX_BREAK_US			176						// Break duration in µs (minimum: 88 µs)
#define DMX_MAB_US				12						// Mark After Break (MAB) duration in µs (minimum: 8 us)
#define DMX_FRAME_INTERVAL_MS	23						// Frame interval in ms (~22.7 ms for DMX512-A)
/** @} */

/**
 * @defgroup Pin_Assignments Default Pin Assignments
 * @{
*/
#ifndef DMX_TX_GPIO_Port
#define DMX_TX_GPIO_Port		GPIOC 				// TX pin for DMX (e.g., SP3485_TX_GPIO_Port)
#define DMX_TX_Pin				GPIO_PIN_1 			// TX pin (e.g., SP3485_TX_Pin)
#define DMX_UART_AF				GPIO_AF8_LPUART1 	// UART alternate function (adjust based on MCU)
#endif

#ifndef DMX_DE_GPIO_Port
#define DMX_DE_GPIO_Port		GPIOC			// DE pin (Driver Enable for MAX485)
#define DMX_DE_Pin				GPIO_PIN_2 		// DE pin (e.g., SP3485_DE_Pin)
#endif

#ifndef DMX_RE_GPIO_Port
#define DMX_RE_GPIO_Port		GPIOC			// RE pin (Receiver Enable for MAX485)
#define DMX_RE_Pin				GPIO_PIN_3 		// RE pin (e.g., SP3485_RE_Pin)
#endif
/** @} */

/**
 * @brief Enable/Disable MAX485 Transmitter
 *
 * Both DE and RE pins are set to HIGH to enable transmission.
*/
#define DMX_ENABLE_TX()  do { \
	HAL_GPIO_WritePin(DMX_DE_GPIO_Port, DMX_DE_Pin, GPIO_PIN_SET); \
	HAL_GPIO_WritePin(DMX_RE_GPIO_Port, DMX_RE_Pin, GPIO_PIN_SET); \
	} while(0)
#define DMX_DISABLE_TX()  do { \
	HAL_GPIO_WritePin(DMX_DE_GPIO_Port, DMX_DE_Pin, GPIO_PIN_RESET); \
	HAL_GPIO_WritePin(DMX_RE_GPIO_Port, DMX_RE_Pin, GPIO_PIN_RESET); \
	} while(0)

/**
 * @defgroup DMX_State_Machine State Machine Definitions
 * @{
*/
typedef enum {
	DMX_IDLE,				// Idle state: waiting for frame interval
	DMX_BREAK,				// Generating Break signal
	DMX_MAB,				// Generating Mark After Break (MAB)
	DMX_TRANSMIT_DATA,		// Transmitting DMX data via DMA
} DMX_State;
/** @} */

/**
 * @defgroup Function_Declarations Function Declarations
 * @{
*/
/**
 * @brief Initialize DMX512 library
 * @param huart Pointer to USART handle (configured for TX)
 * @param htim  Pointer to microsecond timer handle (prescaler set for 1 µs tick)
 * @return HAL_StatusTypeDef
*/
HAL_StatusTypeDef DMX512_Init(UART_HandleTypeDef *huart, TIM_HandleTypeDef *htim);

/**
 * @brief Set a DMX data channel value
 * @param channel Channel number (1-512)
 * @param value   0-255
*/
void DMX512_SetChannel(uint16_t channel, uint8_t value);

/**
 * @brief Set DMX Start Code
 * @param code 8-bit start code
*/
void DMX512_SetStartCode(uint8_t code);

/**
 * @brief Set the entire DMX channel buffer (excluding start code).
 *
 * @param data Pointer to an array of 512 bytes containing channel data.
 */
void DMX512_SetBuffer(const uint8_t *data);

/**
 * @brief Blocking DMX frame send function (not recommended for real-time systems)
*/
void DMX512_SendFrame(void);

/**
 * @brief Non-blocking task to enforce DMX frame interval and transmission
*/
void DMX512_Task(void);
/** @} */

/**
 * @defgroup Getter_Functions Getter Functions
 * @{
*/

/**
 * @brief Returns the current state of the DMX state machine.
 *
 * @return DMX_State Current state (e.g., DMX_IDLE, DMX_BREAK, etc.).
*/
DMX_State DMX512_GetState(void);

/**
 * @brief Checks if a DMX transmission is currently active.
 *
 * @return uint8_t 1 if transmitting, 0 if idle.
*/
uint8_t DMX512_IsTransmitting(void);

/**
 * @brief Starts DMX transmission.
*/
void DMX512_StartTask(void);

/**
 * @brief Stops DMX transmission.
*/
void DMX512_StopTask(void);

/**
 * @brief Reset the DMX buffer to 0 (Start Code + 512 channels).
*/
void DMX512_ResetBuffer(void);

/**
 * @brief Retrieves the value of a specific DMX channel.
 *
 * @param channel Channel number (1-512).
 * @return uint8_t Channel value (0-255). Returns 0 for invalid channels.
*/
uint8_t DMX512_GetChannelValue(uint16_t channel);

/**
 * @brief Retrieves the DMX Start Code (first byte of the DMX buffer).
 *
 * @return uint8_t Start Code value (0x00 for standard DMX).
*/
uint8_t DMX512_GetStartCode(void);

/**
 * @brief Provides a read-only pointer to the entire DMX buffer.
 *
 * @warning Do not modify the buffer directly; use DMX512_SetChannel() instead.
 * @return const uint8_t* Pointer to the DMX buffer (Start Code + 512 channels).
*/
const uint8_t* DMX512_GetBuffer(void);

/**
 * @brief Timer interrupt callback to mark microsecond delay completion, call this function in HAL_TIM_PeriodElapsedCallback
*/
void DMX_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

/**
 * @brief UART transmission complete callback, call this function in HAL_UART_TxCpltCallback
*/
void DMX_UART_TxCpltCallback(UART_HandleTypeDef *huart);

/** @} */
#ifdef __cplusplus
}
#endif

#endif /* DMX512_H_ */
