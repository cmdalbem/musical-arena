#!/bin/sh

FOLDER=fmod
FILE=$FOLDER.zip

# Takes the fmod library name for your computer =D
MYSYSTEM := $(shell uname -m)
ifeq ($(MYSYSTEM), x86_64)
URL="http://www.fmod.org/index.php/release/version/fmodapi43204linux64.tar.gz"
else
URL="http://www.fmod.org/index.php/release/version/fmodapi43204linux.tar.gz"
endif


echo "Downloading FMOD...\n"
wget $URL -O $FILE
tar zxvf $FILE

echo "Done."
