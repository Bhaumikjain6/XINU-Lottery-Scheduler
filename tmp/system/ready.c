/* ready.c - ready */

#include <xinu.h>

qid16	readylist;			/* Index of ready list		*/

qid16	userlist;

/*------------------------------------------------------------------------
 *  ready  -  Make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
status	ready(
	  pid32		pid		/* ID of process to make ready	*/
	)
{
	
	register struct procent *prptr;

	if (isbadpid(pid)) {
		return SYSERR;
	}

	/* Set process state to indicate ready and add to ready list */

	prptr = &proctab[pid];
	prptr->prstate = PR_READY;

	if(prptr->user_process==0){

		insert(pid, readylist, prptr->prprio);
		//kprintf("resched called from ready\n");
		resched();

	}

	else{

		if(prptr->ticket>0 && prptr->completed != 1){
			//kprintf("Here 1 : %d Inserting : %d Runtime : %d\n",prptr->completed,pid,prptr->run_time);
			insert_userproc(pid, userlist,prptr->ticket);
			//insert(pid, userlist, prptr->ticket);
		}
		//kprintf("resched called from ready\n");
		resched();
	}


	return OK;
}
