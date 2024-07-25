#!/bin/bash

if [[ ! -f ${1} ]] ; then
	echo "The first argument should be a file"
	exit 1
fi

text=$(cat ${1} | grep '/home/Inf' | cut -d ',' -f1 | cut -d ':' -f-5 | egrep "a$" | cut -c 3-4 | sort | uniq -c)
echo ${text}


