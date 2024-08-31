#!/bin/bash


if [[ ${#} -ne 3 ]]; then
	echo "Script needs 3 args"
	exit 1
fi

if [[ ! -d ${1} && ! -d ${2} ]]; then
	echo "1rst and 2nd args need to be dirs"
	exit 2
fi

if [[ $UID -ne 0 ]]; then
	echo "Script need to be run from root"
	exit 3
fi

for file in; do
	file_name=$(echo "${file}" | awk -F '/' '{print $NF}')
	echo "${file_name}" | grep -q "${3}"
	if [[ ${?} -eq 0 ]]; then
		dir=$(echo "${file}" | awk -F '/' '{for(i=2;i<NF;i++) printf "%s/",$i}')
		mkdir -p "${DST}/${dir}"
		mv ${file} "${DST}/${dir}"
	fi
done < $(find "${1}" -mindepth 1 -type f)
