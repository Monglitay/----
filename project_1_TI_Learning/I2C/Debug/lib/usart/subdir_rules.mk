################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
lib/usart/%.o: ../lib/usart/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/CCS/ccs/tools/compiler/ti-cgt-armllvm_4.0.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/NUEDC/project_1_TI_Learning/I2C/lib/key" -I"D:/NUEDC/project_1_TI_Learning/I2C/lib/si2c" -I"D:/NUEDC/project_1_TI_Learning/I2C/lib/task" -I"D:/NUEDC/project_1_TI_Learning/I2C/lib/hi2c" -I"D:/NUEDC/project_1_TI_Learning/I2C/lib/usart" -I"D:/NUEDC/project_1_TI_Learning/I2C/app" -I"D:/NUEDC/project_1_TI_Learning/I2C/lib/delay" -I"D:/NUEDC/project_1_TI_Learning/I2C" -I"D:/NUEDC/project_1_TI_Learning/I2C/Debug" -I"C:/TI/mspm0_sdk_2_04_00_06/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_04_00_06/source" -gdwarf-3 -MMD -MP -MF"lib/usart/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


