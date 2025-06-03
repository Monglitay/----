################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
lib/pid/%.o: ../lib/pid/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/CCS/ccs/tools/compiler/ti-cgt-armllvm_4.0.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/key" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/control" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/tracking" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/usart" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/pid" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/delay" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/hc_06" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/buzzer" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/encoder" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/jy62" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/tb6612" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/mpu6050" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/motor" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/task" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/Debug" -I"C:/TI/mspm0_sdk_2_04_00_06/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_04_00_06/source" -gdwarf-3 -MMD -MP -MF"lib/pid/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


