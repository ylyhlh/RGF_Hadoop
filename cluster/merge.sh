#!/bin/bash

#usage: merge.sh test/sampel/train
FILENAME=$1

if [ -e $FILENAME.x -a -e $FILENAME.y ]
then
  echo "Merging $FILENAME.y and $FILENAME.x"
  echo "into"
  echo "$FILENAME.dat"
  # remove \r in \r\n to convert Windows line ending to Unix style
  paste $FILENAME.y $FILENAME.x | tr -d '\r' > $FILENAME.dat
  echo "Merge finish."
else
  echo "merge.sh <prefix of x and y files>"
fi
