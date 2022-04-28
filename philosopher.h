#include <stdio.h>   
#include <pthread.h>  
#include <string> 
#include <unistd.h>   
#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>
#include<vector>
#include <atomic>
#include<chrono>
using namespace std;
using namespace std::chrono;

#define NUMBER_OF_PHILOSOPHERS 5
#define forLoop(y,x) for (int i = y; i < x; i++)

void* philosopher(void* arg);