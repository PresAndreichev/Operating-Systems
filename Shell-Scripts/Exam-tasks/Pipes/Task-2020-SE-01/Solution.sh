#!/bin/bash

find ~ -type f -perm 755 | xargs -I {} chmod g+w 

# find ~ -type f -perm 755 -exec chmod g+W {} \;
