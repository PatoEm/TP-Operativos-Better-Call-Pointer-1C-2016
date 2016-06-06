################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cpu.c \
../src/cpuHandler.c \
../src/mainCpu.c 

OBJS += \
./src/cpu.o \
./src/cpuHandler.o \
./src/mainCpu.o 

C_DEPS += \
./src/cpu.d \
./src/cpuHandler.d \
./src/mainCpu.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2016-1c-Better-call-pointer/parser" -I"/home/utnso/tp-2016-1c-Better-call-pointer/libreriasCompartidas" -I"/home/utnso/tp-2016-1c-Better-call-pointer/commons" -O0 -g3 -Wall -lpthread -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


