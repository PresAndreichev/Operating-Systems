#!/bin/bash

if [[ "$UID" -ne 0 ]]; then
	echo "Script need to be run from root"
	exit 1
fi



rsnotroot=$(ps -e -o uid,pid,user,rss|awk '{if($3!="root"){print $1 " " $2 " " $3 " " $4}}' | awk '{print $3}')

rootMemory=$(ps -e -o uid,rss| awk '{if($1==0){sum+=$2}} END {print sum}')
homedirs=$(cat /etc/passwd | cut -d ':' -f6)
wrongUsers=""

for user in ${rsnotroot}; do
	dir=$(grep  ${user} "${homedirs}")
	if [[ ${?} -ne 0 ]]; then
		wrongUsers+=$(echo "${user} \n")
		continue
	fi
	if [[ ${dir} -id u =~ ${user} ]]; then
		wrongUsers+=$(echo "${user} \n")
		continue
	fi
	getNumber=$(stat -c "%a" ${dir} | cut -c -2)
	if [[ ${getNumber} -ne 4 || ${getNumber} -ne 5 || ${getNumber} -ne 6 || ${getNumber} -ne 7 ]]; then
		wrongUsers+=$(echo "${user} \n")
	fi
done 

for guy in ${wrongUsers}; do
	list=$(ps -e -o user,rssi,uid | grep "^${guy}")
	mem_list=$(echo "${list}" | awk '{sum_list=$2} END {print sum_list}')
	if [[ ${mem_list} -gt ${rootMemory} ]]; then
		for process,rss,id in ${list}; do
			kill "${id}"
		done
	fi
done
