#!/bin/bash

#2. return by value

total=2000

take_sum()
{
   total=0
   for i in $*
   do
   	total=$(($total + $i))
   done
}

take_sum 1 2 3 4  #total is overwitten to 10

echo $total

