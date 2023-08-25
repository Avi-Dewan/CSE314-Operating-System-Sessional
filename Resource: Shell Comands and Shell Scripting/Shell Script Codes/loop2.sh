#!/bin/bash

#here we are using bash
# Bash is more functional than sh

$((0+1))

total=0

for((i=0;i<=50;i++))
do
	total=$(($total + $i))
done

echo $total
