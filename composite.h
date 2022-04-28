#include<iostream>
#include<pthread.h>
#include<vector>
using namespace std;

#define MAX_BUFFER_SIZE 100000000
#define forLoop(y,x) for (int i = y; i < x; i++)

bool isComposite(int n);
void* compositeThread(void* arg);