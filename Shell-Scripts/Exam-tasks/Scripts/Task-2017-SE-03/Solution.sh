#!/bin/bash

if [[ "$UID" -ne 0 ]]; then
	echo "script must be run from root"
	exit 2
fi

process=$(ps -e -o uid,pid,rss --sort=uid | tail -n +2)
first=0
while read uid pid rss; do
	if [[ "${uid}" != "${first}" ]]; then
		avg=$(( mem*2 /counter ))
		if [[ ${maxrss} -gt ${avg} ]]; then
			echo "Kill ${maxpid}"
		fi
		echo "${usr} ${counter} ${mem}"
		usr="${uid}"
		mem=0
		counter=0
		maxpid=0
		maxrss=0
	
	fi
	if [[ ${rss} -gt ${maxrss} ]]; then
		maxrss=${rss}
		maxpid=${pid}
	fi
	mem=$((mem+rss))
	counter=$((counter+1))
done < <(echo "${process}")
