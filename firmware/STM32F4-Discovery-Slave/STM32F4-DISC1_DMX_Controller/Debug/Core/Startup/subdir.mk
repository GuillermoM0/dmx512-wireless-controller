################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f407vgtx.s 

OBJS += \
./Core/Startup/startup_stm32f407vgtx.o 

S_DEPS += \
./Core/Startup/startup_stm32f407vgtx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/Debug" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/Discovery" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Application" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/DMX512" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/NRF24L01" -I"C:/Users/User/STM32CubeIDE/workspace_2/STM32F4-DISC1_TFG_DMX_Controller/Drivers/BSP/protocol" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32f407vgtx.d ./Core/Startup/startup_stm32f407vgtx.o

.PHONY: clean-Core-2f-Startup

