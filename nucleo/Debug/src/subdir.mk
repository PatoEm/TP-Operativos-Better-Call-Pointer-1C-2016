################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cpuHandler.c \
../src/funciones.c \
../src/mainNucleo.c \
../src/nucleo.c 

OBJS += \
./src/cpuHandler.o \
./src/funciones.o \
./src/mainNucleo.o \
./src/nucleo.o 

C_DEPS += \
./src/cpuHandler.d \
./src/funciones.d \
./src/mainNucleo.d \
./src/nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2016-1c-Better-call-pointer/parser" -I"/home/utnso/tp-2016-1c-Better-call-pointer/libreriasCompartidas" -I"/home/utnso/tp-2016-1c-Better-call-pointer/commons" -O0 -g3 -Wall -pthread -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


