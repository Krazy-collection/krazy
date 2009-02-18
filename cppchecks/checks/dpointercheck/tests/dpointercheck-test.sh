#!/bin/bash

# Simple shell script for testing the dpointer check.

function checkDir {
  for f in $( ls $1 ); do
    if [[ -d $1/$f ]]; then
      #echo checking directory: $1/$f;
      checkDir $1/$f;
    elif [[ $f =~ ".h" ]]; then
      echo "Checking header: " $1$f;
      ./dpointercheck $1$f 2> /dev/null
    fi
  done
}

if [[ ! -x dpointercheck ]]; then
  echo "Make sure that you place this script in the same dir where the dpointercheck resides"
  exit 1;
fi

if [[ $# != 1 ]]; then
  echo "Usage: $0 DIRNAME"
  exit 2;
fi

if [[ ! -d $1 ]]; then
  echo "$1 is not a directory";
  exit 3;
fi

echo "Good, starting the test!"
checkDir $1;
