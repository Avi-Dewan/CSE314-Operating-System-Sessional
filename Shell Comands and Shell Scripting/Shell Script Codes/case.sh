#!/bin/sh

echo "Have you eaten?"
read ans

case $ans in
	y*|y|Y|Y*) echo "Good"; echo "better";;
	n*|n|N|N*) echo "Please have these cookies.";;
	*) echo "Can't understand,";;
esac 
