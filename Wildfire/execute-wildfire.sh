#!/bin/bash

# you may need to change execution permission for this script
# chmod u+x execute-wildfire.sh
# afterwards run: ./execute-wildfire.sh

gcc -Wall wildfire-for-students.c -o wildfire.out
./wildfire.out --size=250 --alpha=0.1 --gamma=0.0001 --steps=100
python wildfire.py
