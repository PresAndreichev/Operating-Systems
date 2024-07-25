#!/bin/bash

inodes=$(find . -mindepth 1 -user pesho -exec stat -c "%i %y %n " {} \;|sort -nr -k2 | head -n1 | awk -F ' ' '{print $1}')
echo "$inodes"
