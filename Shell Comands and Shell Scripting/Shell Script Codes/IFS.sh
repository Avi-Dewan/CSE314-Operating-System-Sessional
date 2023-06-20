#!/bin/bash

# Original value of IFS
echo "Default IFS: $IFS"

# Splitting a string into words using the default IFS
string="Hello World"
echo "String: $string"
echo "Splitting using default IFS:"
for word in $string; do
  echo "$word"
done

# Modifying IFS to use a different delimiter (comma)
IFS=,
string="Apple,Orange,Banana"
echo "String: $string"
echo "Splitting using modified IFS (comma):"
for fruit in $string; do
  echo "$fruit"
done


# Set a custom IFS value
IFS=:

# A sample string
string="apple:banana:cherry:grape"

# Split the string into an array using IFS
read -ra fruits <<< "$string"

# Print each element of the array
for fruit in "${fruits[@]}"; do
  echo "$fruit"
done


#!/bin/bash

# Set IFS to comma
IFS=,

# Read and process a CSV file
while IFS= read -r name age country; do
  echo "Name: $name, Age: $age, Country: $country"
done < data.csv

#In this example, IFS is set to a comma (,). The script reads a CSV file named data.csv line by line using a while loop and the read command. The name, age, and country values are extracted from each line, and then the script performs some processing or output based on the data.