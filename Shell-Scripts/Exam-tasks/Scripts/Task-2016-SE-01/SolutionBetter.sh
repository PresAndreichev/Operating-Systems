#!/bin/bash

if [[ ${#} -ne 1 ]]; then
	echo "Need 1 arg only"
	exit 1
fi

if [[ ! -d ${1} ]]; then
	echo "The arg should be directory"
	exit 2
fi

for file in $(find "${1}" -type l -print0 2>/dev/null | xargs -0 -I{} readlink {}); do
	if [[ ! -e ${file} ]]; then
		echo "${file}"
	fi
done
