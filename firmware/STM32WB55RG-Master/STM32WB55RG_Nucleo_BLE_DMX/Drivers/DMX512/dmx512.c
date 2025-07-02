/**
 * @file dmx512.c
 * @brief Implementation of the DMX512 signal generator library.
 * @author Guillermo Morancho
 * @date May 16, 2025
*/

#include "dmx512.h"
#include <string.h>

// DMX Buffer and Configuration
static UART_HandleTypeDef *dmx_huart;				// UART handle for DMX transmission
static TIM_HandleTypeDef  *dmx_htim;				// Timer handle for microsecond delays
static uint8_t dmx_buffer[DMX_BUFFER_SIZE] = {0};	// DMX buffer (Start Code + 512 channels)

// State flags
static volatile uint8_t dma_transfer_complete = 1;	// Flag for DMA completion
static volatile uint8_t delay_complete = 0;			// Flag for microsecond delay completion
static DMX_State dmx_state = DMX_IDLE;				// Current state in the state machine
static uint32_t last_frame_time = 0;				// Timestamp of last frame transmission
static uint8_t DMX_Stop = 1;						// Flag to stop/start transmission
/**
 * @brief Generate a non-blocking microsecond delay using a timer
 * @param us Delay duration in microseconds
*/
static void delay_us(uint32_t us) {
    /* Stop timer if running */
    HAL_TIM_Base_Stop_IT(dmx_htim);
    delay_complete = 0;
    /* Set auto-reload register to desired microseconds */
    __HAL_TIM_SET_AUTORELOAD(dmx_htim, us);
    /* Reset counter */
    __HAL_TIM_SET_COUNTER(dmx_htim, 0);
    /* Start timer in interrupt mode */
    HAL_TIM_Base_Start_IT(dmx_htim);
}


/**
 * @brief Configure TX pin as GPIO output for Break/MAB generation
 * Used during Break (low) and MAB (high) signal generation.
 */
static void DMX512_Tx_Output(void){
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = DMX_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull output
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // No pull-up/pull-down
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // High-speed GPIO
    HAL_GPIO_Init(DMX_TX_GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief Reconfigure TX pin to alternate function for UART transmission
 * Switches from GPIO output to UART mode for data transmission.
 */
static void DMX512_Tx_Alter_Function(void){
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = DMX_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;      // Alternate function push-pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = DMX_UART_AF;    // UART alternate function
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

    // Configure UART for DMX512 (250 kbps, 8N2)
    huart->Init.BaudRate = 250000;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.StopBits = UART_STOPBITS_2;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    if (HAL_UART_Init(huart) != HAL_OK) return HAL_ERROR;

    // Configure Timer for 1 µs ticks
    htim->Init.Prescaler = 32 - 1; // Assuming 32 MHz clock, 1 µs tick
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim->Init.Period = 0xFFFFFFFF;
    htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(htim) != HAL_OK) return HAL_ERROR;
    if (HAL_TIM_Base_Start_IT(htim) != HAL_OK) return HAL_ERROR;

    DMX_Stop = 0; // Enable transmission by default
    return HAL_OK;
}

/**
 * @brief Set a DMX channel value
 * @param channel Channel number (1-512)
 * @param value   0-255
 */
void DMX512_SetChannel(uint16_t channel, uint8_t value) {
    if (channel > 0 && channel <= DMX_MAX_CHANNELS) {
        dmx_buffer[channel] = value; // Update buffer (index 1-512)
    }
}

/**
 * @brief Set the DMX Start Code
 * @param code Start code (0x00 for standard DMX)
 */
void DMX512_SetStartCode(uint8_t code) {
    dmx_buffer[0] = code; // Start Code is at index 0
}

/**
 * @brief Set the entire DMX channel buffer (excluding start code).
 * @param data Pointer to an array of 512 bytes containing channel data.
 */
void DMX512_SetBuffer(const uint8_t *data) {
    if (data == NULL) return;
    memcpy(&dmx_buffer[1], data, DMX_MAX_CHANNELS); // Copy channels 1-512
}

/**
 * @brief Blocking DMX frame send function (not recommended for real-time systems)
 */
void DMX512_SendFrame(void) {
    while (!dma_transfer_complete); // Wait for previous DMA to finish
    DMX_ENABLE_TX(); // Enable transmitter
    DMX512_Tx_Output(); // Set TX pin to GPIO output
    HAL_GPIO_WritePin(DMX_TX_GPIO_Port, DMX_TX_Pin, GPIO_PIN_RESET); // Break: low
    delay_us(DMX_BREAK_US); // Generate Break (non-blocking)
    while (!delay_complete); // Wait for Break to complete

    // MAB: high for 12 µs
    HAL_GPIO_WritePin(DMX_TX_GPIO_Port, DMX_TX_Pin, GPIO_PIN_SET);
    DMX512_Tx_Alter_Function(); // Switch to UART mode
    delay_us(DMX_MAB_US);
    while (!delay_complete);

    // Transmit data via DMA
    dma_transfer_complete = 0;
    HAL_UART_Transmit_DMA(dmx_huart, dmx_buffer, DMX_BUFFER_SIZE);
    if (HAL_UART_GetState(dmx_huart) != HAL_UART_STATE_READY) {
        Error_Handler(); // Handle error
    }
}

/**
 * @brief Non-blocking task to manage DMX frame transmission
 */
void DMX512_Task(void) {
    switch (dmx_state) {
        case DMX_IDLE:
            if (!DMX_Stop) {
                uint32_t current_time = HAL_GetTick();
                if ((current_time - last_frame_time) >= DMX_FRAME_INTERVAL_MS) {
                    last_frame_time = current_time;
                    dmx_state = DMX_BREAK;
                    DMX_ENABLE_TX();
                    DMX512_Tx_Output();
                    HAL_GPIO_WritePin(DMX_TX_GPIO_Port, DMX_TX_Pin, GPIO_PIN_RESET); // Break
                    delay_us(DMX_BREAK_US); // Non-blocking delay
                }
            }
            break;

        case DMX_BREAK:
            if (delay_complete) {
                dmx_state = DMX_MAB;
                HAL_GPIO_WritePin(DMX_TX_GPIO_Port, DMX_TX_Pin, GPIO_PIN_SET); // MAB
                DMX512_Tx_Alter_Function();
                delay_us(DMX_MAB_US);
            }
            break;

        case DMX_MAB:
            if (delay_complete) {
                dmx_state = DMX_TRANSMIT_DATA;
                HAL_UART_Transmit_DMA(dmx_huart, dmx_buffer, DMX_BUFFER_SIZE); // Start DMA
            }
            break;

        case DMX_TRANSMIT_DATA:
            if (dma_transfer_complete) {
                dmx_state = DMX_IDLE; // Return to idle
            }
            break;
    }
}

/**
 * @brief Returns the current state of the DMX state machine.
 */
DMX_State DMX512_GetState(void) {
    return dmx_state;
}

/**
 * @brief Checks if a DMX transmission is currently active.
 */
uint8_t DMX512_IsTransmitting(void) {
    return !DMX_Stop; // 1 if transmitting, 0 if idle
}

/**
 * @brief Starts DMX transmission.
 */
void DMX512_StartTask(void) {
    DMX_Stop = 0; // Enable transmission
}

/**
 * @brief Stops DMX transmission.
 */
void DMX512_StopTask(void) {
    DMX_Stop = 1; // Disable transmission
}

/**
 * @brief Reset the DMX buffer to 0 (Start Code + 512 channels).
 */
void DMX512_ResetBuffer(void) {
    memset(dmx_buffer, 0, DMX_BUFFER_SIZE); // Clear buffer
}

/**
 * @brief Retrieves the value of a specific DMX channel.
 */
uint8_t DMX512_GetChannelValue(uint16_t channel) {
    if (channel > 0 && channel <= DMX_MAX_CHANNELS) {
        return dmx_buffer[channel]; // Return channel value
    }
    return 0; // Invalid channel
}

/**
 * @brief Retrieves the DMX Start Code (first byte of the DMX buffer).
 */
uint8_t DMX512_GetStartCode(void) {
    return dmx_buffer[0]; // Start Code is at index 0
}

/**
 * @brief Provides a read-only pointer to the entire DMX buffer.
 */
const uint8_t* DMX512_GetBuffer(void) {
    return dmx_buffer; // Return buffer pointer
}

/**
 * @brief Timer interrupt callback to mark microsecond delay completion
 */
void DMX_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == dmx_htim) {
        delay_complete = 1; // Mark delay as complete
        HAL_TIM_Base_Stop_IT(htim); // Stop timer
        __HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE); // Clear interrupt flag
    }
}

/**
 * @brief UART transmission complete callback
 */
void DMX_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == dmx_huart) {
        dma_transfer_complete = 1; // Mark DMA as complete
        // DMX_DISABLE_TX(); // Optional: Disable transmitter after transmission
    }
}
