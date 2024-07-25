#!/bin/bash

groupid=$(cat ${1} | sort -t ':' -k1 | head -n204 | tail -n 1 | awk -F ':' '{print $4}')
allusers=$(cat ${1} | egrep ":${groupid}:"| tr ':' ','| awk -F ',' '{print $5 ":" $(NF-1)}')
echo "${allusers}"

