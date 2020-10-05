#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

//#include "scheduler.c"
//#include "dispatcher.c"
#include "commandparser.c"
//#include "performance.c"

pthread_mutex_t queue_lock;
pthread_cond_t queue_not_full;
pthread_cond_t queue_not_empty;
int count = 0;
/**producer**/
void *commandline(void){

    while(1){
        /**getline from stdin and put into Jobqueue if it is not full**/
        char *input_str;
        int input_size = 64;

        input_str = (char*) malloc(input_size * sizeof(char));
        if (input_str == NULL) {
            perror("Unable to malloc buffer");
            exit(1);
        }
        printf(">");
        /**getline(&buffer, &size, stdin);
        &buffer: the address of the 1st character position, pointer-pointer
        &size the address of the varible holds the size of the input buffer
        **/
        getline(&input_str, &input_size, stdin);
        //require mutex
        pthread_mutex_lock(&queue_lock);
        //wait unitl queue is not full.
        //while(job_queue.num == MAXJOB){


        //STEP 3. wait unitl shared_data is not full.
        while(count == MAXJOB){
            pthread_cond_wait(&queue_not_full, &queue_lock);
        }

        count++;


        //cmd dispatcher
        cmd_dispatch(input_str);
        // signal if queue is not empty
        //while(job_queue.num > 0){


        if(count > 0){

       //STEP 4. signal "consumer" that shared_data is not empty.

            pthread_cond_signal(&queue_not_empty);
        }
        //release the resources mutext
        pthread_mutex_unlock(&queue_lock);

    }

}
/**cousumer**/
void *executor(void){
    int i = 0;
//    JobData run_job = JobData_defult;
    /**loop, access to the sharable queue.
    if not empty get job and excute it.
    **/
    for(i = 0; i <= MAXJOB; i++)
    {

        pthread_mutex_lock(&queue_lock);
        //block until satisfied the cv queue_not_empty
        //while(job_queue.num == 0){
        while(count== 0)
            pthread_cond_wait(&queue_not_empty, &queue_lock);
        //get job
        //run_job = GetJob();
        count--;

        pthread_cond_signal(&queue_not_full);
        pthread_mutex_unlock(&queue_lock);

        //RunJob(run_job);
    }

}

  int main(int argc, char *argv[]){
    if(argc == 0){
        printf("usage: csubatch");
        return 0;
    }
    //STEP 0: Replace Dr.Zhou with your group members name.
    printf("Welcome to Dr.Zhou's batch job scheduler Version 1.0\nType 'help' to find more about csubatch commands.\n" );


    pthread_t command_thread, executor_thread;
    char *message1 = "Command Thread";
    char *message2 = "Executor Thread";
    int iret1, iret2;
    /*create thread
    **/
    pthread_create(&command_thread, NULL, commandline, NULL);
    pthread_create(&executor_thread, NULL, executor, NULL);
    /**
    //STEP 1. intialize the condition varible for consumer and producer
    **/
    pthread_mutex_init(&queue_lock, NULL); // create the mutex used by thread
    pthread_cond_init(&queue_not_empty,NULL); // intialize the condition varible for consumer
    pthread_cond_init(&queue_not_full,NULL);  // initalize the condition varible for producer

    //STEP 2. put the producer and consummer threads into the system

    pthread_join(command_thread, NULL);  //put the producer thread into the system
    pthread_join(executor_thread, NULL); // put the consummer thread into the system

    exit(0);


}
