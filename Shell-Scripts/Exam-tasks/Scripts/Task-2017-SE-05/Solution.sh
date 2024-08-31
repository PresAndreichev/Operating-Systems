#!/bin/bash

if [[ ${#} -ne 2 ]]; then
	echo "Script needs 2 args"
	exit 2
fi

if [[ ! -d ${1} ]]; then
	echo "Script needs first arg-dir"
	exit
fi

files=$(find "${1}" -type f -name "vmlinuz-[0-9]+.[0-9]+.[0-9]+-${2}" 2>/dev/null)

files=$(echo "${files}" | sort -t '-' -k2r | head -n1 )
echo "${files}"
