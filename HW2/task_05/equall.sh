#!/bin/bash

file1="testFile/test.txt"
file2="testFile/decoded.txt"

if cmp -s "$file1" "$file2"; then
    printf '"%s" == "%s"\n' "$file1" "$file2"
else
    printf '"%s" != "%s"\n' "$file1" "$file2"
fi
