/**
 * @file holes.c
 * @author Abdullah Saad
 * @date March  20 2021
 * @brief File containing the main file for Simple Memory Management.
 **/

#include <stdio.h>
#define MEMORY_SIZE 128

struct Queue 
{
    char id ;
    int memory_size ;
    int num_swaps ;
    int time ;
    int stateLocation;
    int endLocation ;

    struct Queue * next;
};
typedef struct Queue Queue;

int main(int argc, char *argv[])
{
    //the allocation strategy (algorithm) used
    int mode;

    if (argc != 3)
    {
        printf("Please enter two argument ./holes [Test File] [first, best, next, worst] ");
        return -1 ;
    }

    if (argv[2] == "first")
    {
        // first fit
        mode = 1 ;
    }else if (argv[2] == "best")
    {
        // best fit 
        mode = 2 ;
    }else if (argv[2] == "next")
    {
        // worst fit 
        mode = 3 ;
    }else if (argv[2] == "worst")
    {
        // Next fit
        mode = 4 ;
    }else
    {
        printf("Please enter the algorithm using ./holes [Test File] [first, best, next, worst] ");
        return -1 ;
    }
    // open the file
    FILE * fileName = fopen(argv[1],"r");
    if (!fileName)
    {
        printf("Please enter the File name using ./holes [Test File] [first, best, next, worst] ");
        return -1 ;
    }
    

    Queue * head = NULL ;
    
    
    
    
    
    


    fclose(fileName);

    return 0;
}

/** function take file text and create processes 
*@pre function pointer arguments must not be NULL
*@post List structure has been allocated and initialized
*@return process
*@param filename
*/
Queue * parse(char fileName[]){

    Queue * processList = NULL ;
    char process_ID ;
    char buffer[10];
    char * token ;
    // memory 
    char memory;
    while (fgets(buffer,9,fileName) != NULL)
    {
        Queue * node = NULL ;
        process_ID = buffer[0];
        token = (char*)strtok(buffer," ");
        token = (char*)strtok(NULL,"\n");
        memory = atoi(token);
        node = create_Process(process_ID,memory);
        processList = insert_back(processList,node);

    }
    return processList ;

}
/** function creat new process
*@return new_Process
*@param id - memory_size
*/
Queue * create_Process( char id , int memory_size ){

    // malloc memory for new process 
    Queue * new_Process = malloc(sizeof(Queue));

    new_Process ->id = id ;
    new_Process ->memory_size = memory_size ;
    new_Process ->num_swaps = 0 ;
    new_Process ->time = 0 ;
    new_Process ->stateLocation = 0 ;
    new_Process ->endLocation = 0 ;
    new_Process -> next = NULL ;

    return new_Process ;

}
/** function insert process at the end of the list
*@return processList
*@param processList - node_Add
*/
Queue * insert_back(Queue * processList , Queue * node_Add ){

    Queue * temp = processList ;
    // if no list exists already
    // return the Node ;
    if (processList == NULL)
    {
        return node_Add ;
    }
    // insort at end of the list
    while (temp ->next !=NULL)
    {
        temp = temp -> next ;
    }
    
    processList ->next = node_Add ;
    
    
    return processList ;
    
}

void first_fit( Queue * processList){

    //Memory is of size 128MB.
    int memory[MEMORY_SIZE];
    int time = 0 ;
    int num_of_loads = 0 
    int process_counter = 0 ;
    int hole_counter = 0 ;
    int cumulative_memory = 0 ;

    Queue * memory_Node = NULL ;

    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        memory[i]= 0 ;
    }

    // simulate until all processes in memory 
    while (processList != NULL)
    {
        int space = 0 ;
        int index = -1 ;
        bool space = false ;

        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            /* code */
        }
        
    }
    
    
}