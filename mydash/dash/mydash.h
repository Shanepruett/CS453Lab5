/*
 * Mydash Header File
 *
 * Mydash is a simple shell application that is capable of filename completion
 * command history, executing commands, exiting with ctrl-d or "exit", changing
 * directory with "cd", having a prompt set by the global environment variable
 * DASH_PROMPT, showing the version with "-v", executing process as background
 * processes, mainting a list of running background process, and notifying the 
 * user when a background process has compeleted. 
 *
 * @author: Shane Pruett
 *
 */

#ifndef MYDASH_H
#define MYDASH_H

#include <List.h>


/*
 * This function parses the line in the args in a way that the execvp
 * function will be able to process the exec correctly. The args are
 * used for other functions as well such as cd, and jobs
 *
 * @param: char* line, the string that is entered after the prompt
 * @param: char** args, the parsed arguments will be set in to this char**
 * @return: int, returns the number of arguments
 */
int parseString(char *line, char **args);

/*
 * This function attempts to fork and exec a process according to the args.
 * If the process is meant to run in the background, a job will be created
 * and then added to a list of background jobs.
 *
 * @param: char** args, the array of char* arguments to pass to exec 
 * @param: int background, if background is 1 then its a background
 *         process, if is 0 then it is not.
 * @param: char* unmodLine, this is the string which still contains the &
 *         which is then a part of the commands in a job that is created.
 * @param: ListPtr list, this is the list pointer used to add Nodes to 
 *         the list.
 */
void execute(char **args, int background, char *unmodLine, ListPtr list);

/*
 * This function changes the directory according to the arguments, if no
 * arguments are supplied with the cd then the directory is set to the
 * home directory.
 *
 * @param: char** args, these are the arguments for the change directory
 */
void changeDirectory(char **args);

/*
 * This function takes the list of jobs, steps through them checking to
 * see if each one is done. If the job is done it is printed out, then 
 * removed from the list of jobs.
 *
 * @param: ListPtr list, this is the list of jobs to step through
 */
void printDoneJobs(ListPtr list);

/*
 * This function take the list of jobs and prints out each one, printing
 * whether it is done or still running.
 *
 * @param: ListPtr list, this is the list of jobs to print.
 */
void printAllJobs(ListPtr list);


#endif
