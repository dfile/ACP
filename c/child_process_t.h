#ifndef CHILD_PROCESS_T_H_
#define CHILD_PROCESS_T_H_
// A struct to hold information about a child process, including
// its process ID and pipes.
typedef struct child_process_s                                                                                                                     
{
    pid_t pid;
    int pipes[2];
} child_process_t;
#endif
