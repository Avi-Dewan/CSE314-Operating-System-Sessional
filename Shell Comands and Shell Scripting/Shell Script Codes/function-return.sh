#!/bin/bash

#3. return 


take_sum()
{
   total=0
   for i in $*
   do
   	total=$(($total + $i))
   done
   return $total
}

take_sum 1 2 3 4  

echo $?

take_sum 1 2 3 4  5

a=$?

take_sum 1 2 3 4  5 5 # only 1 place to store it in ? .  keep it store in variable before calling another variable

echo $a $?

