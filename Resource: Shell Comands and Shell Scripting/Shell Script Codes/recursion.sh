#!/bin/bash

# Recursive function to calculate the factorial of a number
factorial() {
    local n=$1

    # Base case: factorial of 0 is 1
    if (( n == 0 )); then
        echo 1
    else
        # Recursive case: n! = n * (n-1)!
        local subfactorial=$((n - 1))
        local subresult=$(factorial "$subfactorial")
        echo $((n * subresult))
    fi
}

# Calculate the factorial of 5
result=$(factorial 5)
echo $result  # Output: 120
