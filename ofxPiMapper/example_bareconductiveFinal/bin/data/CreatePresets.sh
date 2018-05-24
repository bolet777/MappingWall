#!/bin/sh
cd /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductiveFinal/bin/data

INPUT_FILE="presets.xml"
OUTPUT_FILE="presetFinal.xml"

TOKEN1="__PICTURE__.jpg"
TOKEN2="__NAME__.jpg"

length=12
rm -f $OUTPUT_FILE

for i in $( seq 1 $length )
do
	echo "Copy calibration value in "$OUTPUT_FILE
	sed -- "s/$TOKEN1/image$i.jpg/g" $INPUT_FILE | sed -- "s/$TOKEN2/nom$i.jpg/g" >> $OUTPUT_FILE
done
