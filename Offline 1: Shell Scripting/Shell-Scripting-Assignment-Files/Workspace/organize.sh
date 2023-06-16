#!/bin/bash


extract()
{	
	std_id=$2
	targets=$3
	tests=$4
	answers=$5
	verbose=$6
	no_execute=$7
 	if [ -d "$1" ]
 	then 
 		for i in "$1"/*
 		do
 			extract "$i" "$std_id" "$targets" "$tests" "$answers" "$verbose" "$no_execute"
 		done
 	elif [ -f "$1" ]
 	then
 		if [[ $1 == *.c ]]
 		then
 			if [[ $verbose == "true" ]]; then
 				echo "Organizing files of $std_id"
 			fi
 			
 			mkdir "./$targets/C/$std_id"
 			mv $1 "./$targets/C/$std_id/main.c"
 			
 			if [[ $no_execute == "false" ]]; then
 				if [[ $verbose == "true" ]]; then
 					echo "Executing files of $std_id"
 				fi
 				
 				gcc ./targets/C/$std_id/main.c -o ./targets/C/$std_id/main.o
 				
 				matched=0;
				unmatched=0;
 				test_number=1;
 				
 				for test in $(ls ./$tests)
				do
				   
				 	./targets/C/$std_id/main.o < ./tests/$test > "./targets/C/$std_id/out$test_number.txt" 	 
					# Compare the files using diff and store the output in a variable
					diff_output=$(diff "./answers/ans$test_number.txt" "./targets/C/$std_id/out$test_number.txt")
					
					#echo $diff_output

					# Count the number of mismatched lines
					mismatch_count=$(echo "$diff_output" | grep '^<' | wc -l)
					
					if [[ $mismatch_count == 0 ]]; then
						matched=`expr $matched + 1`
					else
						unmatched=`expr $unmatched + 1`
					fi   
					
					test_number=`expr $test_number + 1`	
				done	
				
 				echo "$std_id,C,$matched,$unmatched" >> "./targets/result.csv"
 				
 				
 			fi
 			
 		elif [[ $1 == *.py ]]
 		then	
 			if [[ $verbose == "true" ]]; then
 				echo "Organizing files of $std_id"
 			fi
 			
 			mkdir "./$targets/Python/$std_id"
 			mv $1 "./$targets/Python/$std_id/main.py"
 			#python3 main.py
 			
  			if [[ $no_execute == "false" ]]; then
 				if [[ $verbose == "true" ]]; then
 					echo "Executing files of $std_id"
 				fi
 				
 				
 				matched=0;
				unmatched=0;
 				test_number=1;
 				
 				for test in $(ls ./$tests)
				do
				   	python3 ./targets/Python/$std_id/main.py < ./tests/$test > "./targets/Python/$std_id/out$test_number.txt" 
				   		 
					# Compare the files using diff and store the output in a variable
					diff_output=$(diff "./answers/ans$test_number.txt" "./targets/Python/$std_id/out$test_number.txt")
					
					#echo $diff_output

					# Count the number of mismatched lines
					mismatch_count=$(echo "$diff_output" | grep '^<' | wc -l)
					
					if [[ $mismatch_count == 0 ]]; then
						matched=`expr $matched + 1`
					else
						unmatched=`expr $unmatched + 1`
					fi   
					
					test_number=`expr $test_number + 1`	
				done	
				
 				echo "$std_id,Python,$matched,$unmatched" >> "./targets/result.csv"
 				
 				
 			fi
 			
 		elif [[ $1 == *.java ]]
 		then
 			if [[ $verbose == "true" ]]; then
 				echo "Organizing files of $std_id"
 			fi
 			
 			mkdir "./$targets/Java/$std_id"
 			mv $1 "./$targets/Java/$std_id/Main.java"
 			
 			
			#java -cp ./targets/Java/$std_id Main
			
  			if [[ $no_execute == "false" ]]; then
 				if [[ $verbose == "true" ]]; then
 					echo "Executing files of $std_id"
 				fi
 				
 				javac "./targets/Java/$std_id/Main.java"
 				
 				matched=0;
				unmatched=0;
 				test_number=1;
 				
 				for test in $(ls ./$tests)
				do
				   	java -cp ./targets/Java/$std_id Main  < ./tests/$test > "./targets/Java/$std_id/out$test_number.txt" 
				   		 
					# Compare the files using diff and store the output in a variable
					diff_output=$(diff "./answers/ans$test_number.txt" "./targets/Java/$std_id/out$test_number.txt")
					
					#echo $diff_output

					# Count the number of mismatched lines
					mismatch_count=$(echo "$diff_output" | grep '^<' | wc -l)
					
					if [[ $mismatch_count == 0 ]]; then
						matched=`expr $matched + 1`
					else
						unmatched=`expr $unmatched + 1`
					fi   
					
					test_number=`expr $test_number + 1`	
				done	
				
 				echo "$std_id,Java,$matched,$unmatched" >> "./targets/result.csv"
 				
 				
 			fi
 		fi
 	fi
}


n_arguments=$#


if [[ n_arguments -lt 4 || n_arguments -gt 6 ]]
then
	echo -e "Usage:\n./organize.sh <submission folder> <target folder> <test folder> <answer folder> [-v] [-no execute] \n\n-v: verbose \n-noexecute: do not execute code files"
	kill -INT $$
fi

v=false
no_exec=false

if [[ n_arguments -eq 5 ]]
then
	if [[ $5 == "-v" ]]
	then 
		v=true
	elif [[ $5 == "-noexecute" ]]
	then
	 	no_exec=true
	 fi
elif [[ n_arguments -eq 6 ]]
then
	if [[ (($5 == "-v") && ($6 == "-noexecute"))  || (($5 == "-noexecute") || ( $6 == "-v")) ]]
	then 
		v=true
	 	no_exec=true
	fi
fi


if [[ -d ./$2 ]]; then
	rm -rf "./$2"
fi

mkdir $2
mkdir ./$2/C
mkdir ./$2/Java
mkdir ./$2/Python


if [[ $no_exec == "false" ]]; then
	touch ./targets/result.csv
	echo "student_id,type,mathced,not_matched" >> "./targets/result.csv"
fi

if [[ $v == "true" ]]; then
	count=$(find "./tests" -type f -name "*.txt" | wc -l)
	echo "Found $count test files"
fi


IFS=$'\n'

for file in $(ls ./$1)
do
    if [[ $file == *.zip ]]
    then
 
   	unzip "./$1/$file" > /dev/null -d ./temp 
   	extract ./temp ${file: -11:7} "$2" "$3" "$4" $v $no_exec
   	rm -rf ./temp
   	 
    fi
done
