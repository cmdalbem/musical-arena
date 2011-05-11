#!/bin/sh

# Takes the fmod library name for your computer =D
if [ `uname -m` == 'x86_64' ]
then
	URL='http://www.fmod.org/index.php/release/version/fmodapi43204linux64.tar.gz'
	FOLDER=fmodapi43204linux64
else
	URL='http://www.fmod.org/index.php/release/version/fmodapi43204linux.tar.gz'
	FOLDER=fmodapi43204linux
fi
FILE=$FOLDER.tar.gz

echo "Downloading FMOD...\n"
wget $URL
gzip -dc $FILE | tar xf -

mv $FOLDER fmod
rm $FILE

echo "Done with FMOD."
