#!/bin/bash


count=0

# mkdir $2

file_contents=$(cat virus_list.txt)




visit()
{	
 	if [ -d "$1" ]
 	then 
 		for i in "$1"/*
 		do
            path="$2"
 			visit "$i" "$path"  $3
 		done
 	elif [ -f "$1" ]
 	then
        # echo $1
 		
        # echo $found

        for word in $file_contents; do
            found=$(grep -i -n "rabice" "$1")
            if [ -n "$found" ]; then
                # echo $found
                cp "$2/$1" $3
            fi
        done
 	fi
}


visit ./in_dir "" $2