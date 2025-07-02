################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/protocol/dmx_protocol.c \
../Drivers/protocol/nrf_protocol.c 

OBJS += \
./Drivers/protocol/dmx_protocol.o \
./Drivers/protocol/nrf_protocol.o 

C_DEPS += \
./Drivers/protocol/dmx_protocol.d \
./Drivers/protocol/nrf_protocol.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/protocol/%.o Drivers/protocol/%.su Drivers/protocol/%.cyclo: ../Drivers/protocol/%.c Drivers/protocol/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I../STM32_WPAN/App -I../Utilities/lpm/tiny_lpm -I../Middlewares/ST/STM32_WPAN -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../Middlewares/ST/STM32_WPAN/utilities -I../Middlewares/ST/STM32_WPAN/ble/core -I../Middlewares/ST/STM32_WPAN/ble/core/auto -I../Middlewares/ST/STM32_WPAN/ble/core/template -I../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../Utilities/sequencer -I../Middlewares/ST/STM32_WPAN/ble -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32WB55RG_Nucle_BLE_TFG/Drivers/DMX512" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32WB55RG_Nucle_BLE_TFG/Drivers/NRF24L01" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32WB55RG_Nucle_BLE_TFG/Drivers/protocol" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-protocol

clean-Drivers-2f-protocol:
	-$(RM) ./Drivers/protocol/dmx_protocol.cyclo ./Drivers/protocol/dmx_protocol.d ./Drivers/protocol/dmx_protocol.o ./Drivers/protocol/dmx_protocol.su ./Drivers/protocol/nrf_protocol.cyclo ./Drivers/protocol/nrf_protocol.d ./Drivers/protocol/nrf_protocol.o ./Drivers/protocol/nrf_protocol.su

.PHONY: clean-Drivers-2f-protocol

