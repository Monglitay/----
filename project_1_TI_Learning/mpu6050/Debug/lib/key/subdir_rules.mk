################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
lib/key/%.o: ../lib/key/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/CCS/ccs/tools/compiler/ti-cgt-armllvm_4.0.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O1 -I"D:/NUEDC/project_1_TI_Learning/mpu6050/lib/key" -I"D:/NUEDC/project_1_TI_Learning/mpu6050/lib/usart" -I"D:/NUEDC/project_1_TI_Learning/mpu6050/app/MPU6050" -I"D:/NUEDC/project_1_TI_Learning/mpu6050/lib/delay" -I"D:/NUEDC/project_1_TI_Learning/mpu6050" -I"D:/NUEDC/project_1_TI_Learning/mpu6050/Debug" -I"C:/TI/mspm0_sdk_2_04_00_06/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_04_00_06/source" -DMPU6050 -DMOTION_DRIVER_TARGET_MSPM0 -gdwarf-3 -MMD -MP -MF"lib/key/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


