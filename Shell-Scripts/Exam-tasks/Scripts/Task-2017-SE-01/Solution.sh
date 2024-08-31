#!/bin/bash

if [[ ${#} -lt 2 && ${#} -gt 2 ]]; then
	echo "Script need 1 or 2 args"
	exit 2
fi

if [[ ! -d ${1} ]]; then
	echo "First parameter should be dir"
	exit 2
fi
if [[ ${#} -eq 2 ]]; then
	allFiles=$(find ${1} -type d 2>/dev/null -exec stat -c "%h %n \n" {} \;|awk -v number=${2} '{if(number==$1) {print $2}}')
else
	allFiles=$(find ${1} -type l 2>/dev/null -exec test ! -e {} \; -print) 	
fi

echo "${allFiles}"

