#!/bin/bash
# runs the program 5 times for each specified amount of chopsticks and redirect output to a .txt file
make clean
make all
for i in {1..5}; 
do 
    ./philosopher 5 >> 5-Chopsticks.txt
    sleep 5
done

for i in {1..5}; 
do 
    ./philosopher 6 >> 6-Chopsticks.txt
    sleep 5
done

for i in {1..5}; 
do 
    ./philosopher 7 >> 7-Chopsticks.txt
    sleep 5
done

for i in {1..5}; 
do 
    ./philosopher 8 >> 8-Chopsticks.txt
    sleep 5
done

for i in {1..5}; 
do 
    ./philosopher 9 >> 9-Chopsticks.txt
    sleep 5
done

for i in {1..5}; 
do
    ./philosopher 10 >> 10-Chopsticks.txt
    sleep 5
done