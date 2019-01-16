/* clkhandler.c - clkhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *------------------------------------------------------------------------
 */
void	clkhandler()
{
	static	uint32	count1000 = 1000;	/* Count to 1000 ms	*/

	/* Decrement the ms counter, and see if a second has passed */

	
	if((--count1000) <= 0) {

		/* One second has passed, so increment seconds count */

		clktime++;

		/* Reset the local ms counter for the next second */

		count1000 = 1000;

	}

	ctr1000 = 1000-count1000; //Added by Ajay

	if(proctab[currpid].user_process==1){
		proctab[currpid].run_time--;

		if(proctab[currpid].run_time == 0){

			proctab[currpid].completed = 1;
			resched();
			//kill(currpid);
			//total_tickets -= proctab[currpid].ticket;
			//proctab[currpid].ticket = 0;
			//getitem(currpid);
			//proctab[currpid].prstate = PR_FREE;
			
			//kprintf("Removing tickets : %d\n", total_tickets);
			/*pid32	pid,prev,next;

			pid = getfirst(userlist);
			if(pid==currpid){
					queuetab[pid].qprev = EMPTY;
					queuetab[pid].qnext = EMPTY;
			}
			else{
				while(pid!=currpid){
					pid = queuetab[pid].qnext;
				}
				next = queuetab[pid].qnext;	
				prev = queuetab[pid].qprev;	
				queuetab[prev].qnext = next;
				queuetab[next].qprev = prev;
			}*/
			

		}
		
	}

	/* Handle sleeping processes if any exist */

	if(!isempty(sleepq)) {

		/* Decrement the delay for the first process on the	*/
		/*   sleep queue, and awaken if the count reaches zero	*/

		if((--queuetab[firstid(sleepq)].qkey) <= 0) {
			wakeup();
		}
	}

	/* Decrement the preemption counter, and reschedule when the */
	/*   remaining time reaches zero			     */

	if((--preempt) <= 0) {
		preempt = QUANTUM;
		//if(currpid==7)
			//kprintf("resched called from clkhandler\n");
		resched();
	}
}
