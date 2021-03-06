#!/bin/sh
cd /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductive/bin/data

INPUT_FILE="presets.xml"
OUTPUT_FILE="preset"

TOKEN1="__PICTURE__.jpg"
TOKEN2="__NAME__.jpg"

length=12


for i in $( seq 1 $length )
do
	echo "1) Create file "$OUT
	OUT=$OUTPUT_FILE$i.xml
	cp $INPUT_FILE $OUT

	echo "2) Copy calibration value in "$OUT
	sed -- "s/$TOKEN1/image$i.jpg/g" $INPUT_FILE | sed -- "s/$TOKEN2/nom$i.jpg/g" > $OUT

done
