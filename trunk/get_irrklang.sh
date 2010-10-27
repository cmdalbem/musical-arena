#!/bin/sh

FOLDER=irrKlang-1.3.0b
FILE=$FOLDER.zip
URL=http://www.ambiera.at/downloads/$FILE

echo "Downloading irrKlang...\n"
wget $URL -O $FILE
unzip $FILE

rm -r __MACOSX
rm $FILE

echo "Done."
