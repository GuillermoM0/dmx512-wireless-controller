################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/NRF24L01/nrf24l01.c 

OBJS += \
./Drivers/BSP/NRF24L01/nrf24l01.o 

C_DEPS += \
./Drivers/BSP/NRF24L01/nrf24l01.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/NRF24L01/%.o Drivers/BSP/NRF24L01/%.su Drivers/BSP/NRF24L01/%.cyclo: ../Drivers/BSP/NRF24L01/%.c Drivers/BSP/NRF24L01/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/Debug" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/Discovery" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Application" -I../Core/Inc -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/DMX512" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/NRF24L01" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/protocol" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-NRF24L01

clean-Drivers-2f-BSP-2f-NRF24L01:
	-$(RM) ./Drivers/BSP/NRF24L01/nrf24l01.cyclo ./Drivers/BSP/NRF24L01/nrf24l01.d ./Drivers/BSP/NRF24L01/nrf24l01.o ./Drivers/BSP/NRF24L01/nrf24l01.su

.PHONY: clean-Drivers-2f-BSP-2f-NRF24L01

