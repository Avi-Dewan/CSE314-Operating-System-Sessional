#!/bin/bash

#1. return by echo

take_sum()
{
   total=0
   for i in $*
   do
   	total=$(($total + $i))
   done
   
   echo $total
}

take_sum 1 2 3 4

