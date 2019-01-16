# XINU-Lottery-Scheduler
Project implements Lottery Scheduling in Xinu.

Goal is to implement Lottery Scheduling in Xinu.
As you know, the assumption that a scheduler knows the total execution time of a process a priori is often
unrealistic and not generally applicable. However, in this exercise we will simulate tasks with a predefined
execution time.

1. Writen a custom process creation function create_user_proc that allows the creation of user processes
with a predefined execution time run_time and different CPU usage patterns. This process creation
function does not require explicitly assigning the process a priority. Except for the additional run_time
parameter, create_user_proc should be invoked similarly to the create system call, and it should be
declared as follows:

pid32 create_user_proc(void *funcaddr, uint32 ssize, uint32 run_time, char *name, uint32 nargs, …);
The CPU usage pattern of the user process will depend on that of the function that it executes. 
Implemented the following function:
 void timed_execution(uint32 run_time);
to simulate a CPU intensive function executing for run_time milliseconds.

Both create_user_proc and timed_execution are implemented within create_user_proc.c.

2. Included in the create_user_proc.c file a function:
    void set_tickets(pid32 id, uint32 tickets);
to initialize or (dynamically) modify the number of tickets (tickets) for a given process (identified by id).

3. Utilized the rand() pseudo-random number generator from the stdlib.h library to generate the random
numbers required by the scheduler.

4. Modified Xinu’s scheduler so to allow lottery scheduling of the “user processes”. 

Note that:
• System processes are scheduled with higher priority and not follow the lottery scheduling policy.
• For simplicity, Xinu’s scheduling events are kept, and not limit scheduling decisions to the end of
each time slice.
• User processes with the same number of tickets is scheduled in a round-robin fashion. 
