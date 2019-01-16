#include <xinu.h>
#include <stdio.h>
#include <math.h>

int prA, prB, prC, prD;
int proc_a(), proc_b(), proc_c(), proc_d();
volatile int a_cnt = 0;
volatile int b_cnt = 0;
volatile int c_cnt = 0;
volatile int d_cnt = 0;

int main() {
    double total_cnt, a_percent, b_percent, c_percent, d_percent;
    
    kprintf("\n\n########## Test Case 1, lottery scheduling (2 processes):\n");
    total_cnt=0;
    a_percent=0;
    b_percent=0;
    prA = create_user_proc(proc_a, 1024, 10000, "proc A", 1, 'A');
    prB = create_user_proc(proc_b, 1024, 10000, "proc B", 1, 'B');
    set_tickets(prA, 1000);
    set_tickets(prB, 1000);
    resume(prA);
    resume(prB);
    sleepms(2000);
    kill(prA);
    kill(prB);
    total_cnt = a_cnt + b_cnt ;
    a_percent = (double) a_cnt / total_cnt * 100;
    b_percent = (double) b_cnt / total_cnt * 100;
    kprintf("Test RESULT: A : B (%d : %d)\n", (int) a_percent, (int) b_percent);
    //kprintf("tot:%d\n",total_tickets);

    
    a_cnt = b_cnt = c_cnt = d_cnt = 0;    
    kprintf("\n\n########## Test Case 2, lottery scheduling (2 processes):\n");
    total_cnt=0;
    a_percent=0;
    b_percent=0;
    prA = create_user_proc(proc_a, 1024, 10000, "proc A", 1, 'A');
    //kprintf("pid:%d\n",prA);
    prB = create_user_proc(proc_b, 1024, 10000, "proc B", 1, 'B');
    
    set_tickets(prA, 1000);
    set_tickets(prB, 0);
    resume(prA);
    resume(prB);
    //kprintf("tot:%d\n",total_tickets);
    sleepms(2000);
     
    kill(prA);
    kill(prB);
    total_cnt = a_cnt + b_cnt ;
    a_percent = (double) a_cnt / total_cnt * 100;
    b_percent = (double) b_cnt / total_cnt * 100;
    kprintf("Test RESULT: A : B (%d : %d)\n", (int) a_percent, (int) b_percent);


   
    a_cnt = b_cnt = c_cnt = d_cnt = 0;    
    kprintf("\n\n########## Test Case 3, lottery scheduling (2 processes):\n");
    total_cnt=0;
    a_percent=0;
    b_percent=0;
    prA = create_user_proc(proc_a, 1024, 10000, "proc A", 1, 'A');
    prB = create_user_proc(proc_b, 1024, 500, "proc B", 1, 'B');
    set_tickets(prA, 1000);
    set_tickets(prB, 1000);
    resume(prA);
    resume(prB);
    sleepms(2000);
    kill(prA);
    kill(prB);
    total_cnt = a_cnt + b_cnt ;
    a_percent = (double) a_cnt / total_cnt * 100;
    b_percent = (double) b_cnt / total_cnt * 100;
    kprintf("Test RESULT: A : B (%d : %d)\n", (int) a_percent, (int) b_percent);

    
    a_cnt = b_cnt = c_cnt = d_cnt = 0;    
    kprintf("\n\n########## Test Case 4, lottery scheduling (4 processes):\n");
    total_cnt=0;
    a_percent=0;
    b_percent=0;
    a_cnt=b_cnt=0;
    prA = create_user_proc(proc_a, 1024, 10000, "proc A", 1, 'A');
    prB = create_user_proc(proc_b, 1024, 10000, "proc B", 1, 'B');
    prC = create_user_proc(proc_c, 1024, 10000, "proc C", 1, 'C');
    prD = create_user_proc(proc_d, 1024, 10000, "proc D", 1, 'D');
    set_tickets(prA, 2000);
    set_tickets(prB, 1000);
    set_tickets(prC, 500);
    set_tickets(prD, 250);
    resume(prA);
    resume(prB);
    resume(prC);
    resume(prD);
    sleepms(4000);
    kill(prA);
    kill(prB);
    kill(prC);
    kill(prD);
    total_cnt = a_cnt + b_cnt + c_cnt + d_cnt;
    a_percent = (double) a_cnt / total_cnt * 100;
    b_percent = (double) b_cnt / total_cnt * 100;
    c_percent = (double) c_cnt / total_cnt * 100;
    d_percent = (double) d_cnt / total_cnt * 100;
    kprintf("Test RESULT: A : B : C : D (%d : %d : %d : %d)\n",
            (int) a_percent, (int) b_percent, (int) c_percent, (int) d_percent);


    a_cnt = b_cnt = c_cnt = d_cnt = 0;    
    kprintf("\n\n########## Test Case 5, lottery scheduling (4 processes):\n");
    total_cnt=0;
    a_percent=0;
    b_percent=0;
    a_cnt=b_cnt=0;
    prA = create_user_proc(proc_a, 1024, 10000, "proc A", 1, 'A');
    prB = create_user_proc(proc_b, 1024, 10000, "proc B", 1, 'B');
    set_tickets(prA, 1000);
    set_tickets(prB, 1000);
    resume(prA);
    resume(prB);
    sleepms(2000);
    prC = create_user_proc(proc_c, 1024, 10000, "proc C", 1, 'C');
    prD = create_user_proc(proc_d, 1024, 10000, "proc D", 1, 'D');
    set_tickets(prC, 1000);
    set_tickets(prD, 1000);
    resume(prC);
    resume(prD);
    sleepms(2000);
    kill(prA);
    kill(prB);
    kill(prC);
    kill(prD);
    total_cnt = a_cnt + b_cnt + c_cnt + d_cnt;
    a_percent = (double) a_cnt / total_cnt * 100;
    b_percent = (double) b_cnt / total_cnt * 100;
    c_percent = (double) c_cnt / total_cnt * 100;
    d_percent = (double) d_cnt / total_cnt * 100;
    kprintf("Test RESULT: A : B : C : D (%d : %d : %d : %d)\n",
            (int) a_percent, (int) b_percent, (int) c_percent, (int) d_percent);

    return OK;
}


proc_a(c)
char c; {
    int i;
    while (1) {
        for (i = 0; i < 1000; i++)
            ;
        a_cnt++;
    }
}

proc_b(c)
char c; {
    int i;
    while (1) {
        for (i = 0; i < 1000; i++)
            ;
        b_cnt++;
    }
}

proc_c(c)
char c; {
    int i;
    
    while (1) {
        for (i = 0; i < 1000; i++)
            ;
        c_cnt++;
    }
}

proc_d(c)
char c; {
    int i;
    
    while (1) {
        for (i = 0; i < 1000; i++)
           ;
        d_cnt++;
    }
}
