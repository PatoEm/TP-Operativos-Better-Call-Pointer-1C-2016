################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: swap

dependents:
	-cd /home/utnso/tp-2016-1c-Better-call-pointer/commons/Debug && $(MAKE) all
	-cd /home/utnso/tp-2016-1c-Better-call-pointer/libreriasCompartidas/Debug && $(MAKE) all
	-cd /home/utnso/tp-2016-1c-Better-call-pointer/parser/Debug && $(MAKE) all

# Tool invocations
swap: $(OBJS) $(USER_OBJS) /home/utnso/tp-2016-1c-Better-call-pointer/commons/Debug/libcommons.so /home/utnso/tp-2016-1c-Better-call-pointer/libreriasCompartidas/Debug/liblibreriasCompartidas.so /home/utnso/tp-2016-1c-Better-call-pointer/parser/Debug/libparser.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/tp-2016-1c-Better-call-pointer/parser/Debug" -L"/home/utnso/tp-2016-1c-Better-call-pointer/libreriasCompartidas/Debug" -L"/home/utnso/tp-2016-1c-Better-call-pointer/commons/Debug" -o "swap" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) swap
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/tp-2016-1c-Better-call-pointer/commons/Debug/libcommons.so:
/home/utnso/tp-2016-1c-Better-call-pointer/libreriasCompartidas/Debug/liblibreriasCompartidas.so:
/home/utnso/tp-2016-1c-Better-call-pointer/parser/Debug/libparser.so:

-include ../makefile.targets
