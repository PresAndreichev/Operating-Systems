#!/bin/bash



user=$(find /home -mindepth 1 -type f 2>/dev/null -exec {} stat -c "%Y %n %U" {} \;| sort -k1nr | head -n1 | awk '{print $2 $3}')
echo "${user}"
