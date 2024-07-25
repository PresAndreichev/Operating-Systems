#!/bin/bash

permissions=("600" "644" "700" "755" "777")

for i in "${!permissions[@]}"; do
	filename="file$((i+1)).txt"
	touch "$filename"
	chmod "${permissions[$i]}" "${filename}"
done

for i in "${!permissions[@]}"; do
	dirname="dir$((i+1))"
	mkdir "$dirname"
	chmod "${permissions[$i]}" "$dirname"
done
