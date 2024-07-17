#!/bin/bash

if [ ! -f ${1} ] ; then
	echo "File cant be read"
	exit 1
fi

if [[ ! ${#} -eq 1 ]] ; then
	echo "The list of arguments should be exactly 1"
	exit 2
fi

countLines=$( cat ${1} | egrep "[02468]" | grep -v '[a-w]' | wc -l )

echo 'Number of lines is' ${countLines}

