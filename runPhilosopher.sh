#!/bin/bash
# runs the program for the specified amount of chopsticks and redirect output to a .txt file
make clean
make all

./philosopher 5 >> Chopsticks-Once.txt
sleep 5

./philosopher 6 >> Chopsticks-Once.txt
sleep 5

./philosopher 7 >> Chopsticks-Once.txt
sleep 5

./philosopher 8 >> Chopsticks-Once.txt
sleep 5

./philosopher 9 >> Chopsticks-Once.txt
sleep 5

./philosopher 10 >> Chopsticks-Once.txt