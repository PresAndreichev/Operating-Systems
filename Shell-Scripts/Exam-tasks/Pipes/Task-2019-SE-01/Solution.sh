#!/bin/bash

if [[ ! -f ${1} ]] ; then
	echo "The given arg is not file"
	exit 1
fi

inputData=$(cat ${1} | tail -n +2 |sort -t ';' -k3nr)
findType=$(echo "${inputData}" | head -n1 | awk -F ';' '{print $2}')
close=$(echo "${inputData}"| grep "${findType}" | tail -n 1 | awk -F ';' -v OFS='\t' '{print $1, $4}')
echo "${close}"
