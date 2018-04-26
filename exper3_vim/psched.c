#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
int main(int argc, char *argv[])  {
    int i,j,status;
    int pid[3]; 
    struct sched_param p[3]; 
    for(i=0; i<3;i++){       
        if((pid[i]=fork()) >0){          
            p[i].sched_priority = (argv[i+1] != NULL) ? atoi(argv[i+1]):10;          
            sched_setscheduler(pid[i],(argv[i+4]!=NULL)?atoi(argv[i+4]):SCHED_OTHER,&p[i]);          
            printf("%d  \n",atoi(argv[i+4]));
            setpriority(PRIO_PROCESS,pid[i],(argv[i+1] != NULL) ? atoi(argv[i+1]):10);

        } else{      

            sleep(1);       
            for(i=0; i<10; i++){
                printf("Child PID = %d priority = %d\n",getpid(),getpriority(PRIO_PROCESS,0));
                sleep(1);
            }
            exit( EXIT_SUCCESS);
        }
        
    }    
   
    printf("My child %d policy is %d\n",pid[0],sched_getscheduler(pid[0]));
    printf("My child %d policy is %d\n",pid[1],sched_getscheduler(pid[1]));
    printf("My child %d policy is %d\n",pid[2],sched_getscheduler(pid[2]));
    return EXIT_SUCCESS;
}

