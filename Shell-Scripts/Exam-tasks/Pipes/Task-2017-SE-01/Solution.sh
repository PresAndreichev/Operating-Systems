#!/bin/bash

founded_hard=$(find . -type f -exec stat -c "%h %n" {} \; | sort -nr -k1 | head -n5 )
echo "${founded_hard}"
