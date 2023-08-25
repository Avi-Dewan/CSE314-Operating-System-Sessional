#!/bin/sh

filepath="/home/avi47/DiskC/CSE academic code/3-2/CSE314-Operating-System-Sessional/play"



if [ -d "$filepath" ]
then 
	echo "This is a directory"
else 
	echo "This is a file"
fi
