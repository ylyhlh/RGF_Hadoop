#!/bin/bash

#usage: split.sh <output of merge.sh>
#   eg. split.sh test/sampel/train.dat
FILENAME=$1

if [ -e $FILENAME.x -a -e $FILENAME.y ]
then
  cut -f1 $FILENAME > $FILENAME.y
  cut -f2- $FILENAME > $FILENAME.x
else
  echo "split.sh <output of merge.sh>"
  exit 1
fi
