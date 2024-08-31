#!/bin/bash

if [[ ${#} -ne 2 ]]; then
	echo "Script needs 2 args"
	exit 1
fi

if [[ "${1}" =~ *.csv$ || "${2}" =~ *.csv$ ]]; then
	echo "The two files should be csv format"
	exit 2
fi
sorted=$(cat "${1}" | awk -F ',' '{for(int i=2;i<=NF;i++) {print $i ","} {print $NF}}' | sort | uniq )
for line in; do
	lines=$(cat "${1}" | grep ${line} | awk -F ',' '{print $1}' | sort | head -n1 )
	echo "${lines},${line}" >> ${2}
done < <$(echo "${sorted}")




