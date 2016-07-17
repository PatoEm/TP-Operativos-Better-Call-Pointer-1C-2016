################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/mainUmc.c \
../src/umc.c \
../src/umcTest.c \
../src/umc_server.c \
../src/umc_thread.c 

OBJS += \
./src/mainUmc.o \
./src/umc.o \
./src/umcTest.o \
./src/umc_server.o \
./src/umc_thread.o 

C_DEPS += \
./src/mainUmc.d \
./src/umc.d \
./src/umcTest.d \
./src/umc_server.d \
./src/umc_thread.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2016-1c-Better-Call-pointer/libreriasCompartidas" -I"/home/utnso/tp-2016-1c-Better-Call-pointer/commons" -O0 -g3 -Wall -pthread -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


