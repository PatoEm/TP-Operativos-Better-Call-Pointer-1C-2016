################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libreriasCompartidas/archivosYLogsYMas.c \
../libreriasCompartidas/pcb.c \
../libreriasCompartidas/socket.c 

OBJS += \
./libreriasCompartidas/archivosYLogsYMas.o \
./libreriasCompartidas/pcb.o \
./libreriasCompartidas/socket.o 

C_DEPS += \
./libreriasCompartidas/archivosYLogsYMas.d \
./libreriasCompartidas/pcb.d \
./libreriasCompartidas/socket.d 


# Each subdirectory must supply rules for building sources it contributes
libreriasCompartidas/%.o: ../libreriasCompartidas/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2016-1c-Better-call-pointer/parser" -I"/home/utnso/workspace/tp-2016-1c-Better-call-pointer/commons" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


