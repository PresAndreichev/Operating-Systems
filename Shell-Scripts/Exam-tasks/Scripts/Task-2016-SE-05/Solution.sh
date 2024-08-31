#!/bin/bash

if [[ ${#} -ne 2 ]]; then
	echo "Script needs 2 args"
	exit 1
fi

if [[ ! -f ${1} || ! -f ${2} ]]; then
	echo "Script needs 2 args - files"
	exit 2
fi

fileName1=${1}
fileName2=${2}

lines1=$(cat "${1}" | wc -l)
lines2=$(cat "${2}" | wc -l)

filewhich=""
if [[ ${lines1} -gt ${lines2} ]]; then
	${filewhich}=${1}
else
	${filewhich}=${2}
fi

cat ${filewhich} | awk -F '-' '{print $2 $3}' | sort -k1r > ${filewhicih}.songs
