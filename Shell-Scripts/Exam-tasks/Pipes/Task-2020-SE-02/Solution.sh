#/bin/bash

if [[ ! -f ${1} ]] ; then
	echo "Not a reg file"
	exit 1
fi

comso=$(cat "${1}" | tail -n+2| awk -F '|' '{print $2 " " $3}' | grep "Failure" | sort -k1n | uniq -c |sort -k1n| awk '{print $2}')
result=$(cat "${1}" | tail -n+2 | grep ${comso} | sort -k1n | head -n 1 | awk -F '|' '{print $3 ":" $4}')
echo -e "${result}"
