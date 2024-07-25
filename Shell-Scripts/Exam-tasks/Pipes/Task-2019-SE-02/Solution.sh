#!/bin/bash

userchanged=$(find /home/SI -type d -exec stat -c "%Z %N" {} \; | awk '{if($1>1551168000&&$1<1551176100){print $2}}' | awk -F '/' '{print $4}')
finalresult=""
for line in ${userchanged}; do
	finalresult+=$(cat ${1} | grep "${line}" | awk -F ':' '{print $1 "\t" $5}' | cut -c 1- | awk -F ',' '{print $1}')
	finalresult+='\n'
done
echo -e "${finalresult}"

