#Makefile better call pointer

-include ../makefile.init

-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk
RM := rm -rf

all:
	-cd /home/utnso/tp-2016-1c-Better-call-pointer/libreriasCompartidas/Debug && $(MAKE) clean
	-cd /home/utnso/tp-2016-1c-Better-call-pointer/libreriasCompartidas/Debug && $(MAKE) all
	-cd /home/utnso/tp-2016-1c-Better-call-pointer/swap/Debug && $(MAKE) clean
	-cd /home/utnso/tp-2016-1c-Better-call-pointer/umc/Debug && $(MAKE) clean
	-cd /home/utnso/tp-2016-1c-Better-call-pointer/nucleo/Debug && $(MAKE) clean
	-cd /home/utnso/tp-2016-1c-Better-call-pointer/cpu/Debug && $(MAKE) clean
	-cd /home/utnso/tp-2016-1c-Better-call-pointer/consola/Debug && $(MAKE) clean
	-cd /home/utnso/tp-2016-1c-Better-call-pointer/swap/Debug && $(MAKE) all
	-cd /home/utnso/tp-2016-1c-Better-call-pointer/umc/Debug && $(MAKE) all
	-cd /home/utnso/tp-2016-1c-Better-call-pointer/nucleo/Debug && $(MAKE) all
	-cd /home/utnso/tp-2016-1c-Better-call-pointer/cpu/Debug && $(MAKE) all
	-cd /home/utnso/tp-2016-1c-Better-call-pointer/consola/Debug && $(MAKE) all

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/tp-2016-1c-Better-call-pointer/libreriasCompartidas/Debug/liblibreriasCompartidas.so:
/home/utnso/tp-2016-1c-Better-call-pointer/commons/Debug/libcommons.so:
/home/utnso/tp-2016-1c-Better-call-pointer/parser/Debug/libparser.so:

-include ../makefile.targets
