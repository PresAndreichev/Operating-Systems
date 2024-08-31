#!/bin/bash

if [[ $UID -ne 0 ]]; then #alternative $(id -u)
	echo "The root is not run from root"
	exit 1
fi

if [[ ${#} -ne 1 ]]; then
	echo "The root needs 1 parameter"
	exit 1
fi

psnumber=$(ps -e -o user |tail -n +2 | sort | uniq -c | grep "${1}" | awk '{print $1}')


counter=$(ps -e -o pid | tail -n +2 | wc -l)
for time in $(ps -e -o pid,time| tail -n +2 | awk '{print $2}'); do
	timestamp=$(date -u -d "1970-01-01 ${time}" +%s)
	seconds=$((timestamp % 86400))
	totalSeconds=$((totalSeconds+seconds))
done
totalSeconds=$((totalSeconds/counter))

time_value=(date -u -d "@${totalSeconds}" + '%H:%M:%S')
echo "${time_value}"

totalSeconds=$((totalSeconds*2))

for process in $(ps -e -o user,pid,time | grep "${1}" | tail -n +2); do
	pr=$(echo "${process}" | awk '{print $2}')
	time=$(echo "${process}" | awk '{print $3}')
	timestamp=$(date -d "1970-01-01 ${time}" +%s)
	seconds=$((timestamp % 86400))
	if [[ ${totalSeconds} -lt ${seconds} ]]; then
		echo "${pr}"
	fi
done
