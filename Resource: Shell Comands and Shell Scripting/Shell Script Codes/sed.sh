#!/bin/bash


file_contents=$(cat file.txt)

echo "$file_contents"


replaced=$(sed 's/pattern/replacement/' file.txt)

echo "$replaced"


deleted=$(sed '/pattern/d' file.txt)

echo "$deleted"