#!/bin/bash

num_files=5

for i in $(seq 1 ${num_files}); do
	echo "This is file $i" > "file_$i.txt"
done

for file in file_*.txt; do
	if [ -f "$file" ]; then
		ln "$file" "link_$file"
	fi
done
