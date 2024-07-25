#!/bin/bash

result=$(find . -type f -exec stat -c "%i %n" {} | grep $(find ~ -printf "%T@ %i\n" | sort -nr | head -nr 1 | cut -d ' ' -f2) \ | sort -k 3n | head -n1 | cut -d ' '  -f2)


