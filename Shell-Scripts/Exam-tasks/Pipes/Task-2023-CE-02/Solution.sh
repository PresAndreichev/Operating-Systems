#!/bin/bash

linescount=0
files=$(find / -name "[a-z0-9_]*.log")
for file in ${files}; do
	linescount+=$(grep -o 'error' "$file"| wc -l)
done

echo "${linescount}"
