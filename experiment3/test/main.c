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
    int pid[3]; //Žæ·Åœø³ÌºÅ
    struct sched_param p[3]; //ÉèÖÃµ÷¶È²ßÂÔÊ±Ê¹ÓÃµÄÊýŸÝœá¹¹

    for(i=0; i<3;i++){       //Ñ­»·ŽŽœš 3 žö×Óœø³Ì
        if((pid[i]=fork()) >0){          //È¡œø³ÌÓÅÏÈÊý·ÅÔÚµ÷¶È²ßÂÔÊýŸÝœá¹¹ÖÐ
            p[i].sched_priority = (argv[i+1] != NULL) ? atoi(argv[i+1]):10;          //žžœø³ÌÉèÖÃ×Óœø³ÌµÄµ÷¶È²ßÂÔ.Èç¹ûÃüÁîÐÐµÚ 4,5,6 ²ÎÊýÖž¶šÁË 3 žö²ßÂÔÖµÔò°ŽÖž¶šµÄÊýÉèÖÃ,·ñÔò¶ŒÎªÄ¬ÈÏ²ßÂÔ
            sched_setscheduler(pid[i],(argv[i+4]!=NULL)?atoi(argv[i+4]):SCHED_OTHER,&p[i]);          //žžœø³ÌÉèÖÃ×Óœø³ÌµÄÓÅÏÈÊý,Èç¹ûÃüÁîÐÐµÚ 1,2,3 ²ÎÊýÖž¶šÁË 3 žöÓÅÏÈÊý Ôò°ŽÖž¶šµÄÊýÉèÖÃ,·ñÔò¶ŒÎª 10
            printf("%d  \n",atoi(argv[i+4]));
            setpriority(PRIO_PROCESS,pid[i],(argv[i+1] != NULL) ? atoi(argv[i+1]):10);

        } else{      //ž÷×Óœø³ÌÑ­»·±šžæÆäÓÅÏÈÊýºÍµ÷¶È²ßÂÔ

            sleep(1);       //Ã¿žô 1 Ãî±šžæÒ»ŽÎœø³ÌºÅºÍÓÅÏÈŒ¶
            for(i=0; i<10; i++){
                printf("Child PID = %d priority = %d\n",getpid(),getpriority(PRIO_PROCESS,0));
                sleep(1);
            }
            exit( EXIT_SUCCESS);
        }
        //printf("%d\n",i);
    }    //žžœø³Ì±šžæ×Óœø³Ìµ÷¶È²ßÂÔºóÏÈÐÐÍË³ö
    //wait(NULL);
    printf("My child %d policy is %d\n",pid[0],sched_getscheduler(pid[0]));
    printf("My child %d policy is %d\n",pid[1],sched_getscheduler(pid[1]));
    printf("My child %d policy is %d\n",pid[2],sched_getscheduler(pid[2]));
    return EXIT_SUCCESS;
}
