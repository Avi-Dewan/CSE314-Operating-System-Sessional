#!/bin/bash

visit()
{	
 	if [ -d "$1" ]
 	then 
 		for i in "$1"/*
 		do
 			visit "$i"
 		done
 	elif [ -f "$1" ]
 	then
 		echo "$1"
 	fi
}

visit $1
