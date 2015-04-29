
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "job.h"
#include <common.h>




JobPtr createJob(int number, pid_t id, char *comm)
{

    JobPtr tempJob = malloc(sizeof(Job));
    tempJob->jobNumber = number;
    tempJob->pid = id;
    tempJob->command = comm;

    //printf("inside createJob: %d/n",tempJob->jobNumber);  
    return tempJob;
}

void freeJob(const void *jobObj)
{

    JobPtr object = (JobPtr) jobObj;
    free(object->command);
    free(object);


}


void printJob(const void *jobObj)
{

    JobPtr object = (JobPtr) jobObj;
    printf("[%d] %d %s", object->jobNumber, (int) object->pid,
	   object->command);

}

char *jobToString(const void *jobObj)
{
    JobPtr object = (JobPtr) jobObj;

    char *theString =
	(char *) malloc(sizeof(char) * (strlen(object->command) + 100));

    sprintf(theString, "[%d] %d %s", object->jobNumber, (int) object->pid,
	    object->command);

    return theString;
}

Boolean jobEqual(const void *object, const void *other)
{

    JobPtr objectObj = (JobPtr) object;
    JobPtr otherObj = (JobPtr) other;

    return objectObj->pid == otherObj->pid;

}


void printJobStatus(const void *jobObj)
{

    JobPtr object = (JobPtr) jobObj;
    if (jobDone(jobObj)) {

	printf("[%d] Done %s \n", object->jobNumber, object->command);
    } else {

	printf("[%d] Running %s \n", object->jobNumber, object->command);
    }

}

int jobDone(const void *jobObj)
{

    JobPtr object = (JobPtr) jobObj;
    int statusPtr = NULL;
    return waitpid(object->pid, &statusPtr, WNOHANG);


}
