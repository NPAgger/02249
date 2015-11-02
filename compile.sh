#!/bin/bash
gcc --std=c99 -g3 -Wall -pedantic -O3 -m64 -finline-limit=64 -Iinclude/ source/madragon.c source/bitmatrix.c source/algorithm.c -o madragon