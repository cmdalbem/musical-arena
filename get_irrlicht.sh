#!/bin/sh

FOLDER=irrlicht-1.7.2
FILE=$FOLDER.zip
URL="http://downloads.sourceforge.net/project/irrlicht/Irrlicht%20SDK/1.7/1.7.2/irrlicht-1.7.2.zip?r=http%3A%2F%2Firrlicht.sourceforge.net%2Fdownloads.html&ts=1291305146&use_mirror=ufpr"

echo "Downloading Irrlicht...\n"
wget $URL -O $FILE
unzip $FILE

echo -en "Installing...\n"
sudo apt-get install libxxf86vm-dev
cd $FOLDER/source/Irrlicht
make

cd ../../..
rm $FILE -I

echo "Done with Irrlicht."
