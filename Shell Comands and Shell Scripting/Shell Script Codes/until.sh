#!/bin/bash

read password

until [ $password = "hack" ] 
do
	echo "Password wrong"
	read password
done
