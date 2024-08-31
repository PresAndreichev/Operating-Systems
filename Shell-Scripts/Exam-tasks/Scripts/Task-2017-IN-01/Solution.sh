#!/bin/bash

if [[ ${#} -ne 3 ]]; then
	echo "List of arguments should be 3"
	exit 1
fi

if [[ ! -f  ${1} ]]; then
	echo "First argument should be regular file"
	exit 2
fi

first=$(cat ${1} | grep -E "^${2}=" | awk -F '=' '{print $2}')
second=$(cat ${1} | grep -E "^${3}=" | awk -F '=' '{print $2}')
bla=""
for word in ${second}; do
	if ! echo "${first}" | grep -qw "${word}"; then
		bla+="${word} "
	fi
done
result=""
while read -r line; do
	if echo "${line}" | grep -qE "^${3}="; then
		result+="${3}=$(echo "$bla" | sed 's/ *$//')\n"
	else
		result+="${line}\n"
	fi
done < "$1"
echo -e  "$result" > "$1"


