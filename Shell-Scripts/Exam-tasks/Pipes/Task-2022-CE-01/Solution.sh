#!/bin/bash

find /home/$(whoami) -mindepth 1 -maxdepth 1 -type f 2>/dev/null| xargs -r chmod 664

#or

#find /home/$(whoami) -mindepth 1 -maxdepth 1 -type f -exec chmod 664 {} \; 2>/dev/null
