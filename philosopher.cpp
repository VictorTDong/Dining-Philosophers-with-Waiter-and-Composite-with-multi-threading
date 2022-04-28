/*
Purpose: Philosopher dining problem with waiter 
Known bugs: None
Limitations: Has a limiter on the number of chopsticks the waiter can hold. Chopstick count must be between 5 and 10.
Usage: To make and run code
            Case 1: Single
            make all
            time ./philosopher <Number of chopsticks>
            
            Case 2: Excecuting the program multiple times
            sh runPhilosopher.sh            
            
       To clean files
            make clean
*/
#include <philosopher.h>

/*
Struct: waiterMonitor
Purpose: Monitor for the waiter with helper functions
Variables: 4 semaphores to control the flow of the philosphers
           indicator to indicate that a monitor function is unused
           nextInLine is used to control the queue
           getOneChopstick is used to indicate that there is atleast 1 chopstick availible
           getTwoChopstick is used to indicate that there is atleast 2 chopstick availible
Functions: initialize
            Purpose: Initialize the semaphores

           wait
            Purpose: To wait for the next condition to be unlocked via a signal

           signal
            Purpose: Signals to unblock a thread waiting on the semaphore

           requestLeftChopstick
            Purpose: Request the left chopstick if there is atleast two still availible

           requestRightChopstick
            Purpose: Request the right chopstick if there is atleast one still availible

           returnChopstick
            Purpose: returns both chopsticks and calls signal to indicate there is atleast 1 chopstick availible (note, since we are returning two, we can just
                     call signal on both conditions without checking)

           takedown
            Purpose: To deallocate the semaphores that we initialized
*/
struct waiterMonitor 
{
    sem_t indicator;
    sem_t nextInLine;
    sem_t getOneChopstick;
    sem_t getTwoChopstick;

    int nextCount;
    int chopsticksAvail;
    int getOneChopstickCount;     
    int getTwoChopstickCount; 

    void initialize(int startingChopsticks)
    {
        chopsticksAvail = startingChopsticks;
        getOneChopstickCount = 0;
        getTwoChopstickCount = 0;
        nextCount = 0;

        sem_init(&indicator, 0, 1);
        sem_init(&nextInLine, 0, 0);
        sem_init(&getOneChopstick, 0, 0);
        sem_init(&getTwoChopstick, 0, 0);
    }

    void wait(sem_t &conditionSem, int &conditionCount)
    {
        conditionCount++;
        (nextCount > 0)? sem_post(&nextInLine) : sem_post(&indicator);
        sem_wait(&conditionSem);
        conditionCount--;
    }

    void signal(sem_t &conditionSem, int &conditionCount)
    {
        if(conditionCount > 0)
        {
            nextCount++;
            sem_post(&conditionSem);
            sem_wait(&nextInLine);
            nextCount--;
        }
    }

    void requestLeftChopstick()
    {
        sem_wait(&indicator);

        while (chopsticksAvail < 2)
        {
            wait(getTwoChopstick, getTwoChopstickCount);
        }
        chopsticksAvail--;

        if (chopsticksAvail >= 1)
        {
            signal(getOneChopstick, getOneChopstickCount);
            if (chopsticksAvail >= 2)
            {
                signal(getTwoChopstick, getTwoChopstickCount);
            }
        }
        (nextCount > 0)? sem_post(&nextInLine): sem_post(&indicator);  
    }
    void requestRightChopstick()
    {
        sem_wait(&indicator);

        while (chopsticksAvail < 1)
        {
            wait(getOneChopstick, getOneChopstickCount);
        }
        chopsticksAvail--;

        if (chopsticksAvail >= 1)
        {
            signal(getOneChopstick, getOneChopstickCount);
            if (chopsticksAvail >= 2)
            {
                signal(getTwoChopstick, getTwoChopstickCount);
            }
        }
        (nextCount > 0)? sem_post(&nextInLine): sem_post(&indicator);
    }

    void returnChopstick()
    {
        sem_wait(&indicator);

        chopsticksAvail += 2;

        signal(getOneChopstick, getOneChopstickCount);
        signal(getTwoChopstick, getTwoChopstickCount);
        (nextCount > 0)? sem_post(&nextInLine): sem_post(&indicator);
    }

    void takedown()
    {
        sem_destroy(&indicator);
        sem_destroy(&nextInLine);
        sem_destroy(&getOneChopstick);
        sem_destroy(&getTwoChopstick);
    }
};

double *localAVG; // Holds the avg of each philosopher 
pthread_barrier_t barrier; // Barrier to make sure philisopher doesnt go for seconds until everyone has eaten
atomic<int> canGo; // Signal which philosopher goes from thinking to hungry
atomic<int> done;  // Signal that all philisopher has eaten atleast three times
struct waiterMonitor waiter; // Struct declaration 

int main(int argc, char *argv[])
{   
    //Error checking for the input
    if (argc != 2)
    {
        printf("usage: ./dining <Number of chopsticks>\n");
        return 0;
    }

    int startingChopsticks = atoi(argv[1]); 

    if(5 > startingChopsticks || startingChopsticks > 10)
    {
        printf("Invalid number of chopsticks. Please enter a number between 5-10\n");
        return 0;
    }

    vector<pthread_t> tidArray;
    waiter.initialize(startingChopsticks);
    localAVG = (double*)malloc(NUMBER_OF_PHILOSOPHERS * sizeof(double));

    pthread_barrier_init(&barrier,NULL,5);
    forLoop(0, NUMBER_OF_PHILOSOPHERS)
    {
        pthread_t tid;
        int *arg = new int(i+1);
        pthread_create(&tid, NULL, philosopher, arg);
		tidArray.push_back(tid);
    }

    //Generates a random number to choose a philospher to be hungry
    srand(time(NULL)^abs((int)tidArray[0]));
    while (done < 5)
    {
        canGo = 1 + (rand() % 5);
        sleep(1);
    }
    
    forLoop(0, NUMBER_OF_PHILOSOPHERS)
    {
		pthread_join(tidArray[i], NULL);
	}

    waiter.takedown();
    pthread_barrier_destroy(&barrier);

    double sum = 0;
    printf("Number of chopsticks: %i\nThe average wait time to eat for each philosopher is: \n", startingChopsticks);
    forLoop(0, NUMBER_OF_PHILOSOPHERS)
    {
        printf("Philosopher %i: %f\n", i+1, (localAVG[i]/3));
        sum += localAVG[i]/3;
	}
    printf("The average wait time to eat for all philosopher is: %f\n", sum/5);
    free(localAVG);
}

/*
Function: philosopher
Purpose: Philosopher thread function that lets the philosopher's request chopsticks and eat
Method: Will wait for the main thread to determine which philosopher will be hungry then the philosopher will always request a left then 
        a right chopstick. After the philosopher is finished eating, they will return the chopsticks and waits until everyone has eaten.
Input: void pointer that points to an int pointer for the philosopher number
Output: N/A
*/
void* philosopher(void* arg)
{
    int philosopherID = *((int*) arg);
  
    forLoop(0,3)
    {
        printf("Philosopher %d is thinking\n", philosopherID);
        while (canGo != philosopherID);
        printf("Philospher %d is hungry\n", philosopherID);

        auto start = system_clock::now();
        waiter.requestLeftChopstick();
        printf("Philospher %d has picked up left chopstick\n", philosopherID);

        waiter.requestRightChopstick();
        auto end = system_clock::now();

        printf("Philospher %d has picked up right chopstick\nPhilosopher %d is eating\n", philosopherID, philosopherID); 
        sleep(5);

        printf("Philospher %d is done eating\n", philosopherID);
        waiter.returnChopstick();
        chrono::duration<double> diff = end - start;
        localAVG[philosopherID-1] += (diff.count());
        pthread_barrier_wait(&barrier);
    }
    done++;
    pthread_exit(NULL);
}
