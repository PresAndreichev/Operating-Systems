#!/bin/bash

result=$(find / -type f  -user "$(whoami)" "*.blend*" 2>/dev/null | grep -E '\.blend[0-9]+$')
if [ -n "$result" ]; then
  echo "Files found:"
  echo "$result"
else
  echo "No matching files found."
fi
