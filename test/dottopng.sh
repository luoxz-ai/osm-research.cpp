#!/bin/sh
neato -s1 -n1 -Tpng ../data/$1.dot -o ../data/$1.png
open ../data/$1.png
