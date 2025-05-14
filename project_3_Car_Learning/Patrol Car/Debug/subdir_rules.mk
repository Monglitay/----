################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/CCS/ccs/tools/compiler/ti-cgt-armllvm_4.0.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/key" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/usart" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/pid" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/delay" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/hc_06" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/encoder" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/tb6612" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/mpu6050" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/motor" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/task" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/Debug" -I"C:/TI/mspm0_sdk_2_04_00_06/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_04_00_06/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1009552559: ../main.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/TI/sysconfig_1.22.0/sysconfig_cli.bat" --script "D:/NUEDC/project_3_Car_Learning/Patrol Car/main.syscfg" -o "." -s "C:/TI/mspm0_sdk_2_04_00_06/.metadata/product.json" -d "MSPM0G350X" -p "LQFP-48(PT)" -r "Default" --context "system" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-1009552559 ../main.syscfg
device.opt: build-1009552559
device.cmd.genlibs: build-1009552559
ti_msp_dl_config.c: build-1009552559
ti_msp_dl_config.h: build-1009552559
Event.dot: build-1009552559

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/CCS/ccs/tools/compiler/ti-cgt-armllvm_4.0.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/key" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/usart" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/pid" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/delay" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/hc_06" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/encoder" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/tb6612" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/mpu6050" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/motor" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/task" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/Debug" -I"C:/TI/mspm0_sdk_2_04_00_06/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_04_00_06/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: C:/TI/mspm0_sdk_2_04_00_06/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/CCS/ccs/tools/compiler/ti-cgt-armllvm_4.0.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/key" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/usart" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/pid" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/delay" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/hc_06" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/encoder" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/tb6612" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/mpu6050" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/app/motor" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/lib/task" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car" -I"D:/NUEDC/project_3_Car_Learning/Patrol Car/Debug" -I"C:/TI/mspm0_sdk_2_04_00_06/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_04_00_06/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


