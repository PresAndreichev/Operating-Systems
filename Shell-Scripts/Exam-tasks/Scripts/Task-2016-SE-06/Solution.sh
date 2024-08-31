#!/bin/bash

if [[ ${#} -ne 1 ]]; then
	echo "Script needs 1 arg"
	exit 1
fi

if [[ ! -f ${1} ]]; then
	echo "1 arg should be file"
	exit 2
fi

awk -F '-' '{print NR ":" $2 ":" $3 }' "$1" | sort -t ':' -k2 | awk -F ':' '{print $1 "." $2 "-" $3 $4}' 
