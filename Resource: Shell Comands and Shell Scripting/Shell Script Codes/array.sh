#!/bin/bash

array_name=(2 4 5 32 1)

# array_name[0]=2
# array_name[1]=4
# array_name[2]=5
# array_name[3]=4

echo "${array_name[0]}"

for element in "${array_name[@]}"
do
    echo -n "$element "
done

echo ""

length=${#array_name[@]}

echo "Length: $length"

array_name[0]="new value"

array_name+=("added Value")

for element in "${array_name[@]}"
do
    echo -n "$element "
done

echo ""

unset array_name[3]

echo "Removing index 3: 32"

for element in "${array_name[@]}"
do
    echo -n "$element "
done

echo ""

echo "ARRAY SLICING"

slice=("${array_name[@]:1:4}")

for element in "${slice[@]}"
do
    echo -n "$element "
done

echo ""