/**
 * @file holes.c
 * @author Abdullah Saad
 * @date March  20 2021
 * @brief File containing the main file for Simple Memory Management.
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
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

/* Helper function */
Queue * parse(FILE * fileName);
Queue * create_Process( char id , int memory_size );
Queue * insert_back(Queue * processList , Queue * node_Add );
Queue * pop(Queue ** process);
Queue * push(Queue * processList ,Queue * node , int time , int stat_location , int end_location );
void free_queue(Queue * list);
void remove_process(Queue ** memory_Node ,Queue **processList , int memory[] );
void loading_process(Queue ** memory_Node ,Queue ** processList , int memory[], double * process_counter , double * hole_counter, double * cumulative_memory , int * num_of_loads , int i , int index , int time );
void state( Queue *list , int memory[], int * num_processes, int * num_Holes , double * mem_used);
void first_fit( Queue * processList);
void best_fit( Queue * processList);
void next_fit( Queue * processList);
void worst_fit( Queue * processList);
/***************************************** */

int main(int argc, char *argv[])
{
    //the allocation strategy (algorithm) used
    Queue * head = NULL ; 

    if (argc != 3)
    {
        printf("Please enter two argument ./holes [Test File] [first, best, next, worst] ");
        return -1 ;
    }
    // open the file
    FILE * fileName = fopen(argv[1],"r");
    if (!fileName)
    {
        printf("Please enter the File name using ./holes [Test File] [first, best, next, worst] ");
        return -1 ;
    }
    
    if ( strcmp(argv[2] ,"first") == 0 )
    {
        // first fit
    printf("---------------------------------------  FIRST FIT -------------------------------------\n");
        head = parse(fileName);
        first_fit(head);
    }else if (strcmp(argv[2] ,"best") == 0 )
    {
        // best fit
        printf("------------------------------------ BEST FIT -------------------------------------\n");
        head = parse(fileName);
        best_fit( head);
    }else if (strcmp(argv[2] ,"next") == 0 )
    {
        // next fit
        printf("------------------------------------ NEXT FIT -------------------------------------\n");
        head = parse(fileName); 
        next_fit( head);
    }else if (strcmp(argv[2] ,"worst") == 0 )
    {
        // Next fit
        printf("------------------------------------ Worst FIT -------------------------------------\n");
        head = parse(fileName); 
        worst_fit(head);
    }else
    {
        printf("Please enter the algorithm using ./holes [Test File] [first, best, next, worst] ");
        return -1 ;
    }
    
    fclose(fileName);

    return 0;
}

/** function take file text and create processes 
*@pre function pointer arguments must not be NULL
*@post List structure has been allocated and initialized
*@return process
*@param filename
*/
Queue * parse(FILE * fileName){

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

    if (new_Process == NULL)
    {
        free(new_Process);
        return NULL ;
    }
    
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

    //Queue * temp = processList ;
    // if no list exists already
    // return the Node ;
    if (processList == NULL)
    {
        return node_Add ;
    }
    // insort at end of the list
    else if (processList ->next !=NULL)
    {
        insert_back(processList ->next,node_Add );
    }else
    {
        processList ->next = node_Add ;
    }
    
    
    
    
    
    return processList ;
    
}

void first_fit( Queue * processList){

    //Memory is of size 128MB.
    int memory[MEMORY_SIZE];
    int time = 0 ;
    int num_of_loads = 0 ;
    double process_counter = 0 ;
    double hole_counter = 0 ;
    double cumulative_memory = 0 ;

    Queue * memory_Node = NULL ;

    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        memory[i]= 0 ;
    }


    // simulate until all processes in memory 
    while (processList != NULL )
    {
        int space = 0 ;
        int index = -1 ;
        bool is_space = true ;
        
        
        
        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            
            // if the byte is free 
            if (memory[i] == 0)
            {
                // save it's index 
                if (index == -1)
                {
                    index = i ;
                }
                space = space + 1 ;

                // if there enough room for next process 
                if (space >= processList ->memory_size)
                {
                    loading_process(&memory_Node ,&processList , memory,&process_counter ,&hole_counter,&cumulative_memory , &num_of_loads , i, index , time );

                    is_space = false ;
                    break;
                }
                
                
            }else
            {
                space = 0 ;
                index = -1 ;
            }
            
            
        }
        if (is_space == true)
        {
            
            remove_process(&memory_Node ,&processList , memory);
            
        }
        time ++ ;
        
        
    }

    process_counter /= num_of_loads;
    hole_counter /= num_of_loads ;

    printf("Total loads = %d, average # processes = %lf, average # holes = %lf, cumulative %% mem = %lf \n",num_of_loads,process_counter,hole_counter,cumulative_memory/num_of_loads);
    
    if (memory_Node != NULL)
    {
    
        free_queue(memory_Node);
        
    }
    
}
void best_fit( Queue * processList){

    //Memory is of size 128MB.
    int memory[MEMORY_SIZE];
    int time = 0 ;
    int num_of_loads = 0 ;
    double process_counter = 0 ;
    double hole_counter = 0 ;
    double cumulative_memory = 0 ;

    Queue * memory_Node = NULL ;

    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        memory[i]= 0 ;
    }


    // simulate until all processes in memory 
    while (processList != NULL )
    {
        int space = 0 ;
        int smallest_space = 200;
        int start_index = 0 ;
        int start_index_2 = 0 ;
        int ending_index = 0 ;
        bool not_space = true ;
        
        
        
        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            
            // if the byte is free 
            if (memory[i] == 0 && i != 127)
            {
                // save it's index 
                if (start_index == -1)
                {
                    start_index = i ;
                }
                space = space + 1 ;
                
            }else
            {
                // save it's index 
                if (memory[i] == 0 && i == 127)
                {
                    space = space + 1 ;
                }
                
                // if there enough room for next process 
                if (space < smallest_space && space >= processList->memory_size)
                {
                    smallest_space = space ;
                    not_space = false ; 
                    start_index_2 = start_index ;
                }
                space = 0 ;
                start_index = -1 ;
            }
            
            
        }
        if (not_space == true)
        {
            
            remove_process(&memory_Node ,&processList , memory);
            
        }else
        {
            ending_index = start_index_2 + processList -> memory_size -1 ;
            loading_process(&memory_Node ,&processList , memory,&process_counter ,&hole_counter,&cumulative_memory , &num_of_loads , ending_index, start_index_2 , time );
            not_space = false ;
        }
        
        time ++ ;
        
        
    }

    process_counter /= num_of_loads;
    hole_counter /= num_of_loads ;

    printf("Total loads = %d, average # processes = %lf, average # holes = %lf, cumulative %% mem = %lf \n",num_of_loads,process_counter,hole_counter,cumulative_memory/num_of_loads);
    
    if (memory_Node != NULL)
    {
    
        free_queue(memory_Node);
        
    }
    
}
void next_fit( Queue * processList){

    //Memory is of size 128MB.
    int memory[MEMORY_SIZE];
    int time = 0 ;
    int num_of_loads = 0 ;
    double process_counter = 0 ;
    double hole_counter = 0 ;
    double cumulative_memory = 0 ;

    Queue * memory_Node = NULL ;

    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        memory[i]= 0 ;
    }


    // simulate until all processes in memory 
    while (processList != NULL )
    {
        int space = 0 ;
        int start_index = -1 ;
        int last_index = 0 ;
        bool is_space = true ;
        bool loop = true ;
        
        
        
        for (int i = last_index; i < MEMORY_SIZE; i++)
        {
            
            // if the byte is free 
            if (memory[i] == 0)
            {
                // save it's index 
                if (start_index == -1)
                {
                    start_index = i ;
                }
                space = space + 1 ;

                // if there enough room for next process 
                if (space >= processList ->memory_size)
                {
                    loading_process(&memory_Node ,&processList , memory,&process_counter ,&hole_counter,&cumulative_memory , &num_of_loads , i, start_index , time );

                    is_space = false ;
                    last_index = i ;
                    break;
                }
                
                
            }else
            {
                space = 0 ;
                start_index = -1 ;
            }
            if (i == 127 && is_space == true && loop == true)
            {
                i = 0 ;
                start_index = -1 ;
                space = 0 ;
                loop = false ;
            }
            
            
            
        }
        if (is_space == true)
        {
            
            remove_process(&memory_Node ,&processList , memory);
            
        }
        time ++ ;
        
        
    }

    process_counter /= num_of_loads;
    hole_counter /= num_of_loads ;

    printf("Total loads = %d, average # processes = %lf, average # holes = %lf, cumulative %% mem = %lf \n",num_of_loads,process_counter,hole_counter,cumulative_memory/num_of_loads);
    
    if (memory_Node != NULL)
    {
    
        free_queue(memory_Node);
        
    }
    
}
void worst_fit( Queue * processList){

    //Memory is of size 128MB.
    int memory[MEMORY_SIZE];
    int time = 0 ;
    int num_of_loads = 0 ;
    double process_counter = 0 ;
    double hole_counter = 0 ;
    double cumulative_memory = 0 ;

    Queue * memory_Node = NULL ;

    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        memory[i]= 0 ;
    }


    // simulate until all processes in memory 
    while (processList != NULL )
    {
        int space = 0 ;
        int smallest_space = -200;
        int start_index = 0 ;
        int start_index_2 = 0 ;
        int ending_index = 0 ;
        bool not_space = true ;
        
        
        
        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            
            // if the byte is free 
            if (memory[i] == 0 && i != 127)
            {
                // save it's index 
                if (start_index == -1)
                {
                    start_index = i ;
                }
                space = space + 1 ;

                
                
            }else
            {
                // save it's index 
                if (memory[i] == 0 && i == 127)
                {
                    space = space + 1 ;
                }
                
                // if there enough room for next process 
                if (space > smallest_space && space >= processList->memory_size)
                {
                    smallest_space = space ;
                    not_space = false ; 
                    start_index_2 = start_index ;
                }
                space = 0 ;
                start_index = -1 ;
            
                
            }
        }
        if (not_space == true)
        {
            
            remove_process(&memory_Node ,&processList , memory);
            
        }else
        {
            ending_index = start_index_2 + processList -> memory_size -1 ;
            loading_process(&memory_Node ,&processList , memory,&process_counter ,&hole_counter,&cumulative_memory , &num_of_loads , ending_index, start_index_2 , time );
            not_space = true ;
        }
        
        time ++ ;
        
        
    }

    process_counter /= num_of_loads;
    hole_counter /= num_of_loads ;

    printf("Total loads = %d, average # processes = %lf, average # holes = %lf, cumulative %% mem = %lf \n",num_of_loads,process_counter,hole_counter,cumulative_memory/num_of_loads);
    
    if (memory_Node != NULL)
    {
    
        free_queue(memory_Node);
        
    }
    
}
Queue * pop(Queue ** process){

    Queue * temp = (*process);

    if (!temp)
    {
        return NULL ;
    }
    (*process) = (*process)->next;

    return temp ;
    
}
Queue * push(Queue * processList ,Queue * node , int time , int stat_location , int end_location ){

    node ->time = time ;
    node ->stateLocation = stat_location ;
    node ->endLocation = end_location ;
    node ->next = NULL ;

    processList = insert_back(processList,node);

    return processList;
}
void free_queue(Queue * list){

    if (list ->next != NULL)
    {
        free_queue(list ->next);
    }
    free(list);
    
}
void remove_process(Queue ** memory_Node ,Queue ** processList , int memory[] ){


    

    Queue * remove_process = *memory_Node;
    *memory_Node = (*memory_Node)-> next;
    //memory_Node = memory_Node-> next ;
    
    // mark the memory that used as now free 
    for (int c = remove_process ->stateLocation; c <= remove_process ->endLocation; c++)
    {
        memory[c] = 0 ;
    }
    
    //Once a process has been swapped out for a third time, we assume that the process has
    //run to completion and it is not re-queued.

    if (remove_process->num_swaps != 2)
    {
        remove_process->num_swaps = remove_process->num_swaps + 1 ;  ;
        remove_process ->next = NULL ;
        *processList =insert_back(*processList,remove_process);


    }else
    {
        free(remove_process);
    }
}

                    
void loading_process(Queue ** memory_Node ,Queue ** processList , int memory[], double * process_counter , double * hole_counter, double * cumulative_memory , int * num_of_loads ,int i , int index , int time ){

    
    //////////////
    int num_processes = 0 ;
    int num_Holes = 0 ;
    double   mem_used = 0 ;
    // pop the next process
    // pop(&processList);
    Queue * node = *processList ;
    *processList = (*processList) -> next ;
    // insert process into memory list
    *memory_Node = push(*memory_Node,node, time, index, i );
    // increase loads
    *num_of_loads = *num_of_loads + 1;
    // mark used bytes 
    for (int j = index; j <= i; j++)
    {
        // bytes occupied
        memory[j] = 1 ;
    }

    // stats 
    state( *memory_Node,memory, &num_processes, &num_Holes , &mem_used);
    
    *process_counter =  *process_counter + num_processes ;
    *hole_counter =  *hole_counter + num_Holes ;
    
    double temp_cumulative_memory = *cumulative_memory + mem_used ;
    *cumulative_memory =  *cumulative_memory + mem_used ;
    double  a = ceil(temp_cumulative_memory) / *num_of_loads ;
    // print stat 
    printf("%c loaded, #processes = %d, #holes = %d, %% memusage = %d, cumulative %% mem = %lf \n",node->id,num_processes,num_Holes,(int)round(mem_used),round(a));

}
void state( Queue *list , int memory[], int * num_processes, int * num_Holes , double * mem_used){

    // stats 
    bool hole = false ;
    double  memory_count = 0 ;

    // calculate number of process in memory 
    while (list != NULL)
    {
        *num_processes = (*num_processes + 1) ;
        list = list->next ;
    }
    // count holes and memory use 
    for (int b = 0; b < MEMORY_SIZE; b++)
    {
        // hole count 
        if (memory[b] == 0 && hole == false)
        {
            hole = true ;
            *num_Holes = *num_Holes + 1;
        }
        // count memory use 
        else if (memory[b] == 1)
        {
            hole = false ;
            memory_count++ ;
        }
                        
                        
    }
    // memory util
    *mem_used = ( memory_count/ 128) * 100  ;
    
    
}