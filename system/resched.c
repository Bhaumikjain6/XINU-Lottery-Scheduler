/* resched.c - resched, resched_cntl */

#include <xinu.h>
#include <stdlib.h>
//#include <time.h>

int get_winner( qid16 q){
	//srand(time(0));
	//kprintf("Tickets: %d \n",total_tickets);
	int16	curr;
	curr = firstid(q);
	total_tickets = 0;
	while(curr!=queuetail(q)){
		total_tickets += proctab[curr].ticket;
		curr = queuetab[curr].qnext;
	}

	int winner  = (rand() % total_tickets);

	//kprintf("tot:%d::win:%d\n",total_tickets,winner);
	//kprintf("Winner: %d\n",winner);
	return winner;
}

struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{

	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/
	uint32	stop_time;
	pid32 old_pid,new_pid;


	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if(ptold->user_process == 1){ //Check if reschedule was called by a user process

		if(ptold->prstate == PR_CURR){ //if the user process was running we need to put it into userlist if we are changing it from running to ready

			if(!(strcmp(proctab[firstid(readylist)].prname,"prnull")==0)){ //priority has to be given to system process, so check if there are any process other than prnull in readylist

				//if there is a system process ready other than null insert the userproc in userlist and context switch
				
				//kprintf("here\n");
				old_pid = currpid;
				ptold->prstate = PR_READY;
				//enqueue(currpid, userlist);
				if(ptold->completed != 1){
					//kprintf("Here 2 : %d  Inserting : %d Runtime : %d\n",ptold->completed,currpid,ptold->run_time);
					insert_userproc(currpid, userlist, ptold->ticket);
				}
				else{
						ptold->prstate = PR_FREE;
				}
				currpid = dequeue(readylist);
				new_pid = currpid;
				ptnew = &proctab[currpid];
				ptnew->prstate = PR_CURR;
				preempt = QUANTUM;	
				//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,(clktime*1000+ctr1000));
				#ifdef CTXSW
					kprintf("ctxsw::%d-%d\n",old_pid,new_pid);
				#endif
				//kprintf("Here1\n");	
				ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

			}

			else{ //there are no system process other than prnull in ready state

				if(nonempty(userlist)){ //check if there are other userproc in ready state

					old_pid = currpid;
					ptold->prstate = PR_READY;
					//enqueue(currpid, userlist);
					if(ptold->completed != 1){
						//kprintf("Here 3 : %d  Inserting : %d Runtime : %d\n",ptold->completed,currpid,ptold->run_time);
						insert_userproc(currpid, userlist, ptold->ticket);
					}
					else{
						ptold->prstate = PR_FREE;
					}
					int win = get_winner(userlist);

					currpid = dequeue_userproc(userlist,win);
					//if(ptold->completed==1)
						//kprintf("pid:%d\n",currpid);
					new_pid = currpid;
					ptnew = &proctab[currpid];
					ptnew->prstate = PR_CURR;
					preempt = QUANTUM;
					//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,win);
					if(old_pid!=new_pid){
						//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,(clktime*1000+ctr1000));
						#ifdef CTXSW
							
							kprintf("ctxsw::%d-%d\n",old_pid,new_pid);
						#endif
						ctxsw(&ptold->prstkptr, &ptnew->prstkptr);
					}

					/*if((ptold->ticket < win) && ptold->completed == 0){ //check if there is any userproc with smaller run time than current one

						//if yes then put the current process in userlist and context switch
						old_pid = currpid;
						ptold->prstate = PR_READY;
						enqueue(currpid, userlist);
						//insert(currpid, userlist, ptold->ticket);
						currpid = dequeue_userproc(userlist,win);
						new_pid = currpid;
						ptnew = &proctab[currpid];
						ptnew->prstate = PR_CURR;
						preempt = QUANTUM;
						kprintf("ctxsw::%d-%d\n",old_pid,new_pid);
						kprintf("Here2\n");			
						ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

					}

					else //if there is no other userproc with smaller run time than current proc return without context switch
						return;*/

				}

				else{ //if there are no other userproc in ready state return without context switch
					//kprintf("here\n");
					/*if(!(strcmp(proctab[firstid(readylist)].prname,"prnull")==0))
						kprintf("WTF\n");*/
					//kprintf("here\n");
					preempt = QUANTUM;
					return;
				}

			}

		}

		else{//if the userproc was not in running state and reschedule was called from a userproc

			if(!(strcmp(proctab[firstid(readylist)].prname,"prnull")==0)){ //priority has to be given to system process, so check if there are any process other than prnull in readylist

				old_pid = currpid;
				currpid = dequeue(readylist);
				new_pid = currpid;
				ptnew = &proctab[currpid];
				ptnew->prstate = PR_CURR;
				preempt = QUANTUM;	
				//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,(clktime*1000+ctr1000));
				#ifdef CTXSW
					kprintf("ctxsw::%d-%d\n",old_pid,new_pid);
				#endif
				//kprintf("Here3\n");		
				ctxsw(&ptold->prstkptr, &ptnew->prstkptr);
			}

			else{ //there are no system process to run other than null, so check if there are other userproc to run

				if(nonempty(userlist)){ //if the userproc list is non empty, then call get_winner() to know the winning process and schedule it

					int win = get_winner(userlist);
					old_pid = currpid;
					currpid = dequeue_userproc(userlist,win);
					new_pid = currpid;
					ptnew = &proctab[currpid];
					ptnew->prstate = PR_CURR;
					preempt = QUANTUM;		
					//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,(clktime*1000+ctr1000));
					#ifdef CTXSW
						//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,win);
						kprintf("ctxsw::%d-%d\n",old_pid,new_pid);
					#endif
					//kprintf("Here4\n");		
					ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

				}

				else{ //there are no userproc to run, so run the null process

					old_pid = currpid;
					currpid = dequeue(readylist);
					new_pid = currpid;
					ptnew = &proctab[currpid];
					ptnew->prstate = PR_CURR;
					preempt = QUANTUM;	
					//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,(clktime*1000+ctr1000));
					#ifdef CTXSW
						kprintf("ctxsw::%d-%d\n",old_pid,new_pid);
					#endif
					//kprintf("Here5\n");		
					ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

				}

			}

		}

	}

	else{ //reschdule was called by a system process

		if(!(strcmp(ptold->prname,"prnull")==0)){ //current system process is not null process

			if (ptold->prstate == PR_CURR){ //the state of the system process is running

				if (ptold->prprio > firstkey(readylist)){ //the current process is the highest priority ready process so no context switching

					return;
				}

				else{ //the current process is not the highest priority ready process so need to perform context switching to highest priority ready process

					old_pid = currpid;
					ptold->prstate = PR_READY;
					insert(currpid, readylist, ptold->prprio);
					currpid = dequeue(readylist);
					new_pid = currpid;
					ptnew = &proctab[currpid];
					ptnew->prstate = PR_CURR;
					preempt = QUANTUM;	
					//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,(clktime*1000+ctr1000));
					#ifdef CTXSW
						kprintf("ctxsw::%d-%d\n",old_pid,new_pid);
					#endif
					//kprintf("Here6\n");		
					ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

				}

			}

			else{ //the state of the system process is not running so can perform context switching without checking priority

				if(!(strcmp(proctab[firstid(readylist)].prname,"prnull")==0)){//there is a system process other than null in the ready list

					old_pid = currpid;
					currpid = dequeue(readylist);
					new_pid = currpid;
					ptnew = &proctab[currpid];
					ptnew->prstate = PR_CURR;
					preempt = QUANTUM;		
					//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,(clktime*1000+ctr1000));
					#ifdef CTXSW
						kprintf("ctxsw::%d-%d\n",old_pid,new_pid);
					#endif
					//kprintf("Here7\n");	
					ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

				}

				else{ //there is no system process other than null that is in ready state and the current process is not running so we can run user process if any is ready

					if(nonempty(userlist)){

						
						old_pid = currpid;
						int win = get_winner(userlist);
						//kprintf("win:%d\n",win);
						currpid = dequeue_userproc(userlist,win);
						new_pid = currpid;
						ptnew = &proctab[currpid];
						ptnew->prstate = PR_CURR;
						preempt = QUANTUM;		
						//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,(clktime*1000+ctr1000));
						#ifdef CTXSW
							//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,win);
							kprintf("ctxsw::%d-%d\n",old_pid,new_pid);
						#endif
						//kprintf("Here8\n");	
						ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

					}

					else{ //have to run the null process

						old_pid = currpid;
						currpid = dequeue(readylist);
						new_pid = currpid;
						ptnew = &proctab[currpid];
						ptnew->prstate = PR_CURR;
						preempt = QUANTUM;	
						//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,(clktime*1000+ctr1000));
						#ifdef CTXSW
							kprintf("ctxsw::%d-%d\n",old_pid,new_pid);
						#endif
						//kprintf("Here9\n");		
						ctxsw(&ptold->prstkptr, &ptnew->prstkptr);
					}

				}

			}
		}

		else{ //current system process is null process so we can run user process if there are no system process

			if (ptold->prstate == PR_CURR){ //null process was in running state

				if(isempty(readylist)){ //there are no system process in ready state

					if(nonempty(userlist)){ //there are userproc in ready state so context switch to userproc adding null process to readylist for system process

						int win = get_winner(userlist);
						old_pid = currpid;
						ptold->prstate = PR_READY;
						insert(currpid, readylist, ptold->prprio);
						currpid = dequeue_userproc(userlist,win);
						new_pid = currpid;
						ptnew = &proctab[currpid];
						ptnew->prstate = PR_CURR;
						preempt = QUANTUM;	
						//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,(clktime*1000+ctr1000));
						#ifdef CTXSW
							kprintf("ctxsw::%d-%d\n",old_pid,new_pid);
						#endif
						//kprintf("Here10\n");		
						ctxsw(&ptold->prstkptr, &ptnew->prstkptr);
					}

					else //there is no other user or system process ready to run
						return;
				}

				else{ //there are system process ready to run so give them priority

					old_pid = currpid;
					ptold->prstate = PR_READY;
					insert(currpid, readylist, ptold->prprio);
					currpid = dequeue(readylist);
					new_pid = currpid;
					ptnew = &proctab[currpid];
					ptnew->prstate = PR_CURR;
					preempt = QUANTUM;	
					//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,(clktime*1000+ctr1000));
					#ifdef CTXSW
						kprintf("ctxsw::%d-%d\n",old_pid,new_pid);
					#endif
					//kprintf("Here11\n");			
					ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

				}
			}

			else{ //null process was not in running state

				if(isempty(readylist)){ //there are no system process in ready state

					if(nonempty(userlist)){ //there are userproc in ready state so context switch to userproc

						int win = get_winner(userlist);
						old_pid = currpid;
						currpid = dequeue_userproc(userlist,win);
						new_pid = currpid;
						ptnew = &proctab[currpid];
						ptnew->prstate = PR_CURR;
						preempt = QUANTUM;
						//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,(clktime*1000+ctr1000));
						#ifdef CTXSW
							kprintf("ctxsw::%d-%d\n",old_pid,new_pid);
						#endif
						//kprintf("Here12\n");			
						ctxsw(&ptold->prstkptr, &ptnew->prstkptr);
					}

					else //there is no other user or system process ready to run
						return;
				}

				else{ //there are system process ready to run so give them priority

					old_pid = currpid;
					currpid = dequeue(readylist);
					new_pid = currpid;
					ptnew = &proctab[currpid];
					ptnew->prstate = PR_CURR;
					preempt = QUANTUM;		
					//kprintf("ctxsw::%d-%d (%d)\n",old_pid,new_pid,(clktime*1000+ctr1000));
					#ifdef CTXSW
						kprintf("ctxsw::%d-%d\n",old_pid,new_pid);
					#endif
					//kprintf("Here13\n");	
					ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

				}

			}
		}
	}
	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	
	/*if (ptold->prstate == PR_CURR) {  
		if (ptold->prprio > firstkey(readylist)) {
				
			return;
		}

	

		
		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
	}

	

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);*/

	
	

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
