#!/bin/bash

if [[ ${1} =~ ^-n$ ]]; then
	if [[ ${2} =~ ^[0-9]+$ ]]; then
		N=${2}
	fi
else
	N=10
fi

shift
shift

for file in "${@}"; do
	idf=$(echo "${file}" | awk -F '.' '{for(i=0;i<NF-1;i++) {print $i}}')
	for line in; do
		temp=$(echo "${line}" | awk -F ' ' -v temp=${idf} '{for(f=0;f<NF;f++) {if(f==2) { print $f " " temp} else {print $f} } }')
		echo "${temp}" >> tempfile
	done < $(cat "${file}")
	cat "${tempfile}" | head -n ${N} >> final
done

cat ${final} | sort -k1-2r -t ' '

