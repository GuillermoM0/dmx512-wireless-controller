################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Discovery/led.c \
../Drivers/BSP/Discovery/pushbutton.c 

OBJS += \
./Drivers/BSP/Discovery/led.o \
./Drivers/BSP/Discovery/pushbutton.o 

C_DEPS += \
./Drivers/BSP/Discovery/led.d \
./Drivers/BSP/Discovery/pushbutton.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Discovery/%.o Drivers/BSP/Discovery/%.su Drivers/BSP/Discovery/%.cyclo: ../Drivers/BSP/Discovery/%.c Drivers/BSP/Discovery/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/Debug" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/Discovery" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Application" -I../Core/Inc -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/DMX512" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/NRF24L01" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/protocol" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Discovery

clean-Drivers-2f-BSP-2f-Discovery:
	-$(RM) ./Drivers/BSP/Discovery/led.cyclo ./Drivers/BSP/Discovery/led.d ./Drivers/BSP/Discovery/led.o ./Drivers/BSP/Discovery/led.su ./Drivers/BSP/Discovery/pushbutton.cyclo ./Drivers/BSP/Discovery/pushbutton.d ./Drivers/BSP/Discovery/pushbutton.o ./Drivers/BSP/Discovery/pushbutton.su

.PHONY: clean-Drivers-2f-BSP-2f-Discovery

