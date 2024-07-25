#!/bin/bash

lines_count=$(find / -exec stat -c "%U" {} + 2>/dev/null | grep -c "^$(whoami)$")
echo ${lines_count}
