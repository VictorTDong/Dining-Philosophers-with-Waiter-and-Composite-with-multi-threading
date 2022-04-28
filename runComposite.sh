#!/bin/bash
make clean
make all
for i in 1 2 4 8 16 32 64 128 256; 
do 
    time ./composite $i 2
    sleep 5
done
