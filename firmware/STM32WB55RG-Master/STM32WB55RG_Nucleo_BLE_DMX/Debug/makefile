################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Utilities/sequencer/subdir.mk
-include Utilities/lpm/tiny_lpm/subdir.mk
-include STM32_WPAN/Target/subdir.mk
-include STM32_WPAN/App/subdir.mk
-include Middlewares/ST/STM32_WPAN/utilities/subdir.mk
-include Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/subdir.mk
-include Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci/subdir.mk
-include Middlewares/ST/STM32_WPAN/ble/svc/Src/subdir.mk
-include Middlewares/ST/STM32_WPAN/ble/core/template/subdir.mk
-include Middlewares/ST/STM32_WPAN/ble/core/auto/subdir.mk
-include Drivers/protocol/subdir.mk
-include Drivers/STM32WBxx_HAL_Driver/Src/subdir.mk
-include Drivers/NRF24L01/subdir.mk
-include Drivers/DMX512/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := STM32WB55RG_Nucleo_BLE_DMX
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
STM32WB55RG_Nucleo_BLE_DMX.elf \

MAP_FILES += \
STM32WB55RG_Nucleo_BLE_DMX.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
STM32WB55RG_Nucleo_BLE_DMX.list \


# All Target
all: main-build

# Main-build Target
main-build: STM32WB55RG_Nucleo_BLE_DMX.elf secondary-outputs

# Tool invocations
STM32WB55RG_Nucleo_BLE_DMX.elf STM32WB55RG_Nucleo_BLE_DMX.map: $(OBJS) $(USER_OBJS) C:\Users\User\STM32CubeIDE\workspace_2\STM32WB55RG_Nucle_BLE_TFG\STM32WB55RGVX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "STM32WB55RG_Nucleo_BLE_DMX.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\User\STM32CubeIDE\workspace_2\STM32WB55RG_Nucle_BLE_TFG\STM32WB55RGVX_FLASH.ld" --specs=nosys.specs -Wl,-Map="STM32WB55RG_Nucleo_BLE_DMX.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

STM32WB55RG_Nucleo_BLE_DMX.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "STM32WB55RG_Nucleo_BLE_DMX.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) STM32WB55RG_Nucleo_BLE_DMX.elf STM32WB55RG_Nucleo_BLE_DMX.list STM32WB55RG_Nucleo_BLE_DMX.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
