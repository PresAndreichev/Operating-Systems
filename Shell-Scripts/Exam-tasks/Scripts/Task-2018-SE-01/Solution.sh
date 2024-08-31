#!/bin/bash

if [[ ${#} -ne 1 || ! -d ${1} ]]; then
	echo "Script need 1 arg - dir"
	exit 1
fi


allfriend=$(find "${1}" -type d -maxdepth 4 -mindepth 4) 

for one in ${allfriend}; do
	txt_files
done

