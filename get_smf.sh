#!/bin/sh

FOLDER=libsmf-1.3
FILE=$FOLDER.tar.gz
URL="http://downloads.sourceforge.net/project/libsmf/libsmf/1.3/libsmf-1.3.tar.gz?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Flibsmf%2Ffiles%2Flibsmf%2F1.3%2Flibsmf-1.3.tar.gz%2Fdownload&ts=1287948805&use_mirror=ufpr"

echo "Downloading libsmf...\n"
wget $URL -O $FILE
gzip -dc $FILE | tar xf -

echo -en "Installing...\n"
cd $FOLDER
./configure
make
sudo make install

sudo ldconfig /usr/local/lib

cd ..
rm $FILE -I
rm -r $FOLDER -I

echo "Done."
