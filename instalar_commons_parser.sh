#!/bin/bash
#Scrpit para instalar las commons y el parser perro

echo ">>> Ingrese contraseña:"
sudo echo ">>> Gracias."

# Instalación de utnso commons
cd ~
echo ">>> Git init"
git init
echo ">>> Clonando commons"
git clone "https://github.com/sisoputnfrba/so-commons-library"
cd so-commons-library
echo ">>> Instalando commons"
sudo make all
sudo make install

# Instalación del parser
cd ~
echo ">>> Clonando parser"
git clone "https://github.com/sisoputnfrba/ansisop-parser"
cd ansisop-parser
cd parser
echo ">>> Instalando parser"
sudo make all
sudo make install

echo ">>> Probando parser"
cd ~
cd ansisop-parser
cd so-ansisop-parser-dummy
sudo make all
./build/so-ansisop-parser-dummy

echo ">>> FIN"
