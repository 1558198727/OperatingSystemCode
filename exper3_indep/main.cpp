#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <signal.h>
typedef void (*sighandler_t)(int);
void sigcat()
{
//+1
    setpriority(PRIO_PROCESS, getpid(), getpriority(PRIO_PROCESS, getpid()) + 1);
}
void sigzat()
{
//-1
    setpriority(PRIO_PROCESS, getpid(), getpriority(PRIO_PROCESS, getpid()) -1);
    //kill(getpid()+1,SIGUSR1);

}
void child_zat(){

}

int main(int argc, char *argv[])
{
    int i,j,status;
    int pid; //存放进程号
    struct sched_param p[2]; //设置调度策略时使用的数据结构
    //for(i=0; i<2; i++)
    //{
//取进程优先数放在调度策略数据结构中
        p[1].sched_priority = 9;
        p[0].sched_priority = 11;

    //}
    pid = fork();
    if(pid < 0)
    {
        printf("failed!");
    }
    else if(pid > 0)
    {
        //SIGINT===》ctrl+C SIGTST===》ctrl+Z
        signal(SIGINT, (sighandler_t)sigcat);


        sched_setscheduler(getpid(), SCHED_OTHER, &p[0]);
        setpriority(PRIO_PROCESS, getpid(), 11);
        //sleep(2);
        for(i = 0; i < 10; i++)
        {
            printf("parent PID = %d, priority = %d, scheduler = %d\n", getpid(),getpriority(PRIO_PROCESS, 0), sched_getscheduler(getpid()));
            printf("parent process hang on ，press Ctrl + C add 1 to priority \n");
            //pause();
            sleep(1);

        }
    }else{//各子进程循环报告其优先数和调度策略
        //SIGINT===》ctrl+C SIGTST===》ctrl+Z
        signal(SIGINT, (sighandler_t)sigzat);

        sched_setscheduler(getpid(), SCHED_OTHER, &p[1]);
        setpriority(PRIO_PROCESS, getpid(), 9);


        for(i=0; i<10; i++)
        {
            printf("Child PID = %d priority= %d scheduler = %d\n",getpid(),getpriority(PRIO_PROCESS,0), sched_getscheduler(getpid()));
            printf("child prosecc hang on，press Ctrl + Z jie 1 to priority\n");
            //pause();
            sleep(1);

        }
        exit( EXIT_SUCCESS);
    }
    return EXIT_SUCCESS;
}
