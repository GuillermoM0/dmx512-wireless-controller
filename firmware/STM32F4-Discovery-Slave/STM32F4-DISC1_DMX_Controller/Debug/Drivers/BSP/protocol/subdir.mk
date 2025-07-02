################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/protocol/dmx_protocol.c \
../Drivers/BSP/protocol/nrf_protocol.c 

OBJS += \
./Drivers/BSP/protocol/dmx_protocol.o \
./Drivers/BSP/protocol/nrf_protocol.o 

C_DEPS += \
./Drivers/BSP/protocol/dmx_protocol.d \
./Drivers/BSP/protocol/nrf_protocol.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/protocol/%.o Drivers/BSP/protocol/%.su Drivers/BSP/protocol/%.cyclo: ../Drivers/BSP/protocol/%.c Drivers/BSP/protocol/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/Debug" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/Discovery" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Application" -I../Core/Inc -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/DMX512" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/NRF24L01" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/protocol" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-protocol

clean-Drivers-2f-BSP-2f-protocol:
	-$(RM) ./Drivers/BSP/protocol/dmx_protocol.cyclo ./Drivers/BSP/protocol/dmx_protocol.d ./Drivers/BSP/protocol/dmx_protocol.o ./Drivers/BSP/protocol/dmx_protocol.su ./Drivers/BSP/protocol/nrf_protocol.cyclo ./Drivers/BSP/protocol/nrf_protocol.d ./Drivers/BSP/protocol/nrf_protocol.o ./Drivers/BSP/protocol/nrf_protocol.su

.PHONY: clean-Drivers-2f-BSP-2f-protocol

