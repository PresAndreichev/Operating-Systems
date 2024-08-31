#!/bin/bash

if [[ ${#} -ne 2 ]]; then
	echo "Script needs 2 args"
	exit 1
fi

if [[ ! -d ${1} || ! find ${1} -mindepth 1 -maxdepth 1 ]]; then
	echo "First arg should be empty dir"
	exit 1
fi

if [[ ! -f ${2} ]]; then
	echo "Second arg should be regfile"
	exit 2
fi

mkdir -p "${1}/dict.txt"

num=1

for line in; do
	replica=$( echo "${line}" | awk -F ':' '{print $2}')
	user=$(echo "${line}"  | awk -F '(' '{print $1}' | egrep "^[a-zA-Z0-9]+ [a-zA-Z0-9]+" | awk -F ' ' '{print $1 " " $2}')
	number=$(cat "${1}/dict.txt" | grep "${user}" | awk -F ';' '{print $2}')
	if [[ ${?} -ne 0 ]]; then
		echo "${user};${num}" >> "${1}/dict.txt"	
		mkdir -p "${1}/${num}"
		echo "${replica} \n " > "${1}/${num}"
	else
		echo "${replica} \n" >> "${1}/${number}"
	fi
	num=$((num+1))
done< <(cat "${1}")
