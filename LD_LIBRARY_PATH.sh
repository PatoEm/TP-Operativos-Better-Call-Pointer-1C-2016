#!/bin/bash
# Script de Better call pointer

cd ~

echo "# seteo el library path 
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/tp-2016-1c-Better-call-pointer/libreriasCompartidas/Debug:~/tp-2016-1c-Better-call-pointer/commons/Debug:~/tp-2016-1c-Better-call-pointer/parser/Debug" >> .bashrc

echo "Library Path de Better call pointer agregada"

cp ~/tp-2016-1c-Better-call-pointer/nucleo/confignucleo ~/tp-2016-1c-Better-call-pointer/nucleo/Debug
cp ~/tp-2016-1c-Better-call-pointer/umc/configumc ~/tp-2016-1c-Better-call-pointer/umc/Debug
cp ~/tp-2016-1c-Better-call-pointer/consola/configconsola ~/tp-2016-1c-Better-call-pointer/consola/Debug
cp ~/tp-2016-1c-Better-call-pointer/cpu/configcpu ~/tp-2016-1c-Better-call-pointer/cpu/Debug
cp ~/tp-2016-1c-Better-call-pointer/swap/swapconfig ~/tp-2016-1c-Better-call-pointer/swap/Debug

