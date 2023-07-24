#!/bin/bash
find . -type f -perm -111 -exec file {} \; |grep ELF |awk -F: '{print $1}' |xargs -I {} rm {}
