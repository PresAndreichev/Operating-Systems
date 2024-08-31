#!/bin/bash


declare -a arra

read -a arra
sum=0
# or index=0
# while [[ "$index" -lt "${#arra[@]}" ]]; do
#	...
#	index=$index+1 or ((index++))
# done
for element in $(seq 0 $((${#arra[@]} -1 ))); do
	if ! [[ ${element} =~ ^[0-9]+$ ]]; then
		continue;
	elif [[ ${element} -lt 0 ]]; then
		sum=$sum-${element}
	else
		sum=$sum+${element}
	fi
done

for again in $(seq 0 $((${arra[@]} - 1 ))); do
	if [[ ${arra[${again}]} -eq ${sum} ]]; then
		echo "${arra[${again}]} \n"
		continue
	fi
	again1=$again-$((2*${again}))
	if [[ ${again1} -eq ${sum} ]]; then
		echo "${again} \n"
	fi
done
