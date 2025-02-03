################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/subdir.mk
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


BUILD_ARTIFACT_NAME := SPWM_Singlephase_Inverter
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
SPWM_Singlephase_Inverter.elf \

MAP_FILES += \
SPWM_Singlephase_Inverter.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
SPWM_Singlephase_Inverter.list \

OBJCOPY_HEX += \
SPWM_Singlephase_Inverter.hex \


# All Target
all: main-build

# Main-build Target
main-build: SPWM_Singlephase_Inverter.elf secondary-outputs

# Tool invocations
SPWM_Singlephase_Inverter.elf SPWM_Singlephase_Inverter.map: $(OBJS) $(USER_OBJS) C:\Users\Mohammad\STM32CubeIDE\workspace_1.17.0\SPWM_Singlephase_Inverter\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "SPWM_Singlephase_Inverter.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\Mohammad\STM32CubeIDE\workspace_1.17.0\SPWM_Singlephase_Inverter\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="SPWM_Singlephase_Inverter.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

SPWM_Singlephase_Inverter.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "SPWM_Singlephase_Inverter.list"
	@echo 'Finished building: $@'
	@echo ' '

SPWM_Singlephase_Inverter.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "SPWM_Singlephase_Inverter.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) SPWM_Singlephase_Inverter.elf SPWM_Singlephase_Inverter.hex SPWM_Singlephase_Inverter.list SPWM_Singlephase_Inverter.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
