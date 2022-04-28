/*
Purpose: Multithreading and calculating the number of composite numbers in a randomly generate number sequence 
Known bugs: None
Limitations: Has a limiter on the number of threads this program can run. Thread count must be between 1 to 256.
Usage: To make and run code
            Case 1: Single
            make all
            time ./composite <Number of Threads> <Seed>
            
            Case 2: Incrementing the threads automatically from 1-256 on seed 2
            sh runComposite.sh           
            
       To clean files
            make clean
*/
#include <composite.h>

int compositeArray[MAX_BUFFER_SIZE]; //int array holding all the numbers to be checked
int *localCount; //int pointer that points to the count of each thread
int numberOfThreads; //int for the number of threads
vector<pthread_t> tidArray; //vector to hold all the thread ID's

int main(int argc, char const *argv[])
{
    int sum = 0;

    if (argc != 3)
    {
        printf("usage: ./composite <Number of Threads> <Seed>\n");
        return 0;
    }

    numberOfThreads = atoi(argv[1]);  
    if(1 > numberOfThreads || numberOfThreads > 256)
    {
        printf("Invalid number of threads. Please enter a number between 1-256\n");
        return 0;
    }

    int numberLimit = 50000;
    srand(atoi(argv[2]));
    localCount = (int*)malloc(numberOfThreads * sizeof(int));

    forLoop(0, MAX_BUFFER_SIZE)
    {
        compositeArray[i] = rand() % numberLimit;
    }
    
    forLoop(0, numberOfThreads)
    {
        pthread_t tid;
        int *arg = new int(i);
        pthread_create(&tid, NULL, compositeThread, arg);
		tidArray.push_back(tid);
    }
 
    forLoop(0, numberOfThreads)
    {
		pthread_join(tidArray[i], NULL);
        sum += localCount[i];
	}
    printf("\nRunning seed %i with %i threads and a range of %i\nThe number of composite numbers in the array is %i/%i", atoi(argv[2]), numberOfThreads, numberLimit,sum, MAX_BUFFER_SIZE);
	return 0;
}

/*
Function: compositeThread
Purpose: Composite thread that counts the number of composite numbers in a certain range
Method: Checks to see which thread it is that joined. Will then calculate the range based on the thread.
        If it is the last thread then the remainder will also be added onto the range to ensure that all
        numbers are accounted for. After it creates the range, it will run through each number in that range
        and determines if it is a composite number and will add it to the count if it is.
Input: void pointer that points to an int pointer representing the thread number
Output: N/A
*/
void* compositeThread(void* arg){
	int index = *(int*) arg;
	int lower = index * (MAX_BUFFER_SIZE/numberOfThreads);
	int upper = min((index + 1) * (MAX_BUFFER_SIZE/numberOfThreads), MAX_BUFFER_SIZE);

    if (index == (numberOfThreads - 1))
    {
        upper += MAX_BUFFER_SIZE % numberOfThreads; 
    }
    
    forLoop(lower,upper)
    {
		if(isComposite(compositeArray[i]))
        {
            localCount[index]++;
        }
	}
	pthread_exit(0);
}

//cite: https://www.geeksforgeeks.org/composite-number/
//      A optimized school method based C++ program to check if a number is composite.
bool isComposite(int n)
{
	if (n <= 1 || n <= 3)
    {
        return false;
    }

	if (n%2 == 0 || n%3 == 0)
    {
        return true;
    }

	for (int i = 5; i*i <= n; i += 6)
    {
		if (n%i == 0 || n%(i + 2) == 0)
        {
		    return true;
        }
    }
	return false;
}