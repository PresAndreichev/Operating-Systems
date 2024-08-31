#!/bin/bash

if [[ ${#} -lt 1 && ${#} -gt 2 ]]; then
	echo "Script needs 1 or 2 args"
	exit 2
fi

allFiles=$(find ${1} -type l 2>/dev/null)
for file in ${allFiles}; do
	if [[ ! -e ${file} ]]; then
		counter=$((counter+1))
	else
		result+=$(echo "${file} ->")
		result+=$(readlink -f ${file})
		result+="\n"
	fi
done

if [[ ${#} -eq 2 ]]; then
	echo "${result}" > ${2}
	echo "Total broken symlinks: ${counter}" > ${2}
else
	echo "${result}"
	echo "Total Broekn Symlinks: ${counter}"
fi

