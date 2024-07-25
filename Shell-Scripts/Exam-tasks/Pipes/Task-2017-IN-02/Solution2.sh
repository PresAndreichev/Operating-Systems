#!/bin/bash

find . -type f -size 0 2>/dev/null -exec rm -f {}\;

find . -type f exec du -b {} \; | sort -nr -k1 | head -n 5 | cut -f2 | xargs rm
