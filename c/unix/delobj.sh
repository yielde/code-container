#!/bin/bash

find . -type f -perm -111 -exec file {} \; |grep arm64 |awk -F: '{print $1}' |xargs -I {} rm {}
find . -type d -name "*.dSYM" -exec rm -rf {} \;
