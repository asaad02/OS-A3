/**
 * @file dine.c
 * @author Abdullah Saad
 * @date March  20 2021
 * @brief File containing the main file for Dining Philosophers problem.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

/* A thread acquires the lock before entering a critical section and releases it upon
exiting the critical section.
*/
#include <pthread.h>
pthread_mutex_t chopsticks[1000];
int Philosophers_NUM;
int eating ;



void *table(void *arg);
void dinning(int num_Philosophers, int num_Times_Eats);

// solution to the dining philosopher's problem.
int main(int argc, char const *argv[])
{
    //Your program should take two command--line arguments

    
    if (argc != 3)
    {
        printf("Invalid arguments ./dine [philosophers] [Timers of philosophers Should eat\n");
        return -1 ;
    }
    printf("\n");
    printf("-----------------------\n");
    // variables 
    // number of philosophers
    int num_Philosophers = atoi(argv[1]);
    // Number of times each philosopher should eat
    int num_Times_Eats = atoi(argv[2] ) * atoi(argv[1]);
    //The number of philosophers (an integer larger than 2)
    //The number of times each philosopher should eat (an integer in the range 1..1000).
    if (num_Philosophers <= 2 || num_Times_Eats < 1 || num_Times_Eats > 1000 )
    {
        printf("Invalid arguments { Need more than 2 Philosophers and number of Eats between 1-1000\n");
        return -1 ;
    }else
    {
        // begin simulation 
        dinning(num_Philosophers,num_Times_Eats);
    }
    printf("-----------------------\n");
    
    
    

    return 0;
}
void dinning(int num_Philosophers, int num_Times_Eats){

    // create array of threads 
    pthread_t Philosophers[num_Philosophers];
    //struct Philo philoArray[num_Philosophers];
    
    Philosophers_NUM = num_Philosophers;
    eating = num_Times_Eats ;

    long i = 0 ;
    // create and initialize the mutex lock
    while(i < num_Philosophers)
    {
        //default mutex attributes are used as second attribute 
        pthread_mutex_init(&chopsticks[i],NULL);
        i++ ;
        //philoArray[i].index = i;
    }
    // running each thread
    // create and initialize the mutex lock

    

    long c = 0 ;
    while (c < num_Philosophers)
    {
        // pthread_create() function is used to create a new thread
        //If successful, the pthread_create() function returns zero. Otherwise
        //an error number is returned to indicate the error.
        if (pthread_create(&Philosophers[c],NULL,table,(void*)c))
        {
            printf("Thread failed to create \n");
            exit(1) ;
        }
        c ++ ; 
        
        
    }
    // all thread terminated before exiting
    for (int j = 0; j < num_Philosophers; j++)
    {
        if (pthread_join(Philosophers[j],NULL))
        {
            printf("Thread failed to create \n");
            exit(1) ;
        }
        
    }
    
    
}
void *table(void *arg){

    //struct Philo *p = (struct Philo*)arg;
    //long index = (long)p -> index  ;
    long id = (long)arg ;
    int start = 0 ;
    //printf("eating before loop %d \n",eating);
    while (eating > 0)
    {
        //printf("eating after loop %d \n",eating);
        int index = 0 ;

        if (id == 0 )
        {
            index = Philosophers_NUM - 1 ;
        }else{
            index = id - 1;
        }
        

        //pthread mutex lock() is invoked, 
        //the calling thread is blocked until the owner invokes pthread mutex unlock()

        int left = pthread_mutex_lock(&chopsticks[id]);
        int right = pthread_mutex_lock(&chopsticks[index]);

        if (start == 0)
        {
            printf("Philosopher %ld is Thinking ..\n",id + 1);
            start = 1 ;
        }

        if (left == 0 && right == 0 )
        {
            printf("Philosopher %ld is Eating ..\n",id + 1);
            eating -- ;
            sleep(1);
            
        }
        
        
        printf("Philosopher %ld is Thinking ..\n",id + 1);
        //the mutex is acquired and released with the pthread mutex lock()
        pthread_mutex_unlock(&chopsticks[id]);
        pthread_mutex_unlock(&chopsticks[index] );
        
        sleep(1);

    }
    pthread_exit(NULL);
    return NULL;
    

}

