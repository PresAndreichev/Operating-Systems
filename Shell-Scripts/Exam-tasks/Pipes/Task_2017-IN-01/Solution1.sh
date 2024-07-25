#!/bin/bash


lines_count=$(find / -user $(whoami) 2>/dev/null | wc -l)
echo ${lines_count}
