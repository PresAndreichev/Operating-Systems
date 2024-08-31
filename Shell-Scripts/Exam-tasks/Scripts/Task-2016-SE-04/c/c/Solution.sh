#!/bin/bash

if [[ ${#} -ne 2 ]]; then
	echo "Script needs 2 args"
	exit 1
fi

if ! [[ "$1" =~ ^-?[0-9]+$ ]]; then
	echo "First arg is not a number"
	exit 2
fi

if ! [[ "$2" =~ ^-?[0-9]+$ ]]; then
	echo "Second arg is not a number"
	exit 2
fi

mkdir "a"
mkdir "b"
mkdir "c"

for file in *; do
	if [[ -f "${file}" ]]; then
			
		lines=$(cat "${file}" | wc -l)
		if [[ ${lines} -lt ${1} ]]; then
			mv ./"${file}" ./a/"${file}"
		elif [[ ${lines} -ge ${1} && ${lines} -le ${2} ]]; then
			mv ./"${file}" ./b/"${file}"
		else 
			mv ./"${file}" ./c/"${file}"
		fi
	fi
done
