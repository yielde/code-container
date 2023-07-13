#!/bin/bash

find . -type f -executable -exec file {} \; |grep ELF |awk -F: '{print $1}' |xargs -I {} rm {}