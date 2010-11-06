#!/bin/sh

TESTSTMP=`mktemp`
PLUGINSTMP=`mktemp`
EXTRASTMP=`mktemp`
SRCTMP=`mktemp`
CHECKSTMP=`mktemp`
NODATA=`mktemp`
NOTEST=`mktemp`

find . -mindepth 3 -type d | sort > $TESTSTMP

cd ..
find ./plugins/ -mindepth 2 -type f ! -name Makefile ! -name description.txt > $PLUGINSTMP
find ./extras/ -mindepth 2 -type f ! -name Makefile ! -name description.txt > $EXTRASTMP
find ./src/ -mindepth 1 -maxdepth 1 -type d ! -name "desktop-file-utils-*" > $SRCTMP
cd - > /dev/null

cat $PLUGINSTMP $EXTRASTMP $SRCTMP | sort > $CHECKSTMP


diff -u $CHECKSTMP $TESTSTMP > /dev/null

DIFFRET=$?

if [ $DIFFRET != 0 ]; then
  diff -u $CHECKSTMP $TESTSTMP | grep -e '^-\.' | sed -e 's/^-\.\///' > $NODATA
  printf "%s\n" "------------------------------------------"
  printf "%s\n" "The following checks don't have test data:"
  printf "%s\n" "------------------------------------------"
  cat $NODATA
fi

if [ $DIFFRET != 0 ]; then
  diff -u $CHECKSTMP $TESTSTMP | grep -e '^+\.' | sed -e 's/^+\.\///' > $NOTEST
  printf "%s\n" "-------------------------------------------------"
  printf "%s\n" "The following test data don't belong to any test:"
  printf "%s\n" "-------------------------------------------------"
  cat $NOTEST
fi


trap "rm -rf $TESTSTMP $PLUGINSTMP $EXTRASTMP $SRCTMP $CHECKSTMP $NODATA $NOTEST" EXIT
