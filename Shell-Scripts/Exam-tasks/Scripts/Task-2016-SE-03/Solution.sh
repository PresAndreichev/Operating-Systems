#!/bin/bash

if [[ $(id -u) -ne 0 ]]; then
	Echo "Script must be run as root"
	exit 1
fi

for file in $(cat /etc/passwd | awk -F ':' '{if($6!="") print $6}'); do
	if [[ ! -d ${file} ]]; then
		perm=$(stat -c "%a" ${file} | cut -c 1)
		if [[ ${perm} -ne 7 && ${perm} -ne 6 && ${perm} -ne 4 ]]; then
			user=$(stat -c "%u" ${file})
			echo "${user}"
		fi
	fi
done

