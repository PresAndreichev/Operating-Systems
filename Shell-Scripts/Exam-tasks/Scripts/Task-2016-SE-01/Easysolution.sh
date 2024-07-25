#!/bin/bash

if [[ ${#} -ne 1 ]]; then
	echo "One arg for this script"
	exit 1
fi
if [[ ! -d ${1} ]]; then
	echo "Not a directory"
	exit 2
fi
find "${1}" -type l -exec test ! -e \; -print
