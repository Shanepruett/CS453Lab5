/*
 * Job Header File
 *
 * Job is used to keep track of process. Each job stores a number
 * associated with the job, a process id, and the original command
 * used to execute the process.
 *
 *
 * @author: Shane Pruett
 *
 */




#ifndef JOB_H
#define JOB_H

#include <common.h>


/*
 * The job struct is used to store all the pertinent information
 * about the process.
 *
 */
struct job {
    int jobNumber;
    pid_t pid;
    char *command;
};

typedef struct job Job;
typedef struct job *JobPtr;

/*
 * This function returns a JobPtr to a job created using the
 * parameters associated with the process.
 *
 * @param: int number, the number associated with the job
 * @param: pid_t id, the process id of the process
 * @param: char* comm, the command used to execute the process
 * @return: JobPtr, pointer to the newly created struct
 */
JobPtr createJob(int number, pid_t id, char *comm);

/*
 * This function checks to see if the job is complete
 *
 * @param: const void* jobObj, the JobPtr to check to see if its complete
 * @return: int, if the job is not complete it returns 0
 */
int jobDone(const void *jobObj);

/*
 * This function prints a message stating whether the job is don
 * or still running.
 *
 * @param: const void* jobObj, the JobPtr to print the status of
 *
 */
void printJobStatus(const void *jobObj);

/*
 * This function prints out information about the job
 *
 * @param: const void* jobObj, the pointer to the job to print
 *
 */
void printJob(const void *jobObj);

/*
 * This function frees the memory used by command and job.
 *
 * @param: const void* jobObj, the pointer to the job to free
 *
 */
void freeJob(const void *jobObj);

/*
 * This function returns a string representation of the job
 *
 * @param: const void *jobObj, the job to get the string representation of
 * @return: char*, the string that represents the job.
 */
char *jobToString(const void *jobObj);

/*
 * This function compares the jobs to evaluate if they are the same,
 * if the jobs have the same pid, then they are equal and this 
 * will return true.
 *
 * @param: const void* object, the first job to compare
 * @param: const void* other, second job to compare
 * @return: Boolean, if they are equal then >0 otherwise 0;
 *
 */
Boolean jobEqual(const void *object, const void *other);








#endif
