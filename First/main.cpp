#include<sys/types.h>
#include<wait.h>
#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
typedef void(*sighandler_t)(int);
void sigcat()
{
    printf("%d Process continue\n",getpid());

}
int main(int argc,char*argv[])
{

    int status_1,status_2;
    signal(SIGINT,(sighandler_t)sigcat);//Registe a interrupt fuction
    char *args1[]= {"/bin/ls","-a",NULL}; //两个进程
    char *args2[]= {"/bin/ps","-a",NULL};
    int pid1=fork();
    if(pid1<0)
    {
        printf("Create Process fail\n");
    }
    if(pid1==0)
    {
        printf("ls -the child process starting%d\n",getpid());
        pause();//Wait for the interrupt
        printf("ls the child process waking%d\n",getpid());
        status_1=execve(args1[0],args1,NULL);
        exit(0);

    }
    else
    {
        //Father process
        printf("\n Father Process starting%d\n ",getpid());
        int pid2=fork();
        if(pid2>0)
        {

            printf("ls the childprocess pause%d\n",pid1);
            waitpid(pid2,&status_2,0);
            printf("pid2 is over");
            printf("ls waking%d\n",pid1);
            kill(pid1,SIGINT);//运行p1
            waitpid(pid1,&status_1,0);
            printf("ls over%d\n",pid1);
            printf("Father process over%d\n",getpid());
            exit(0);

        }
        if(pid2<0)
        {
            printf("Process 2fails\n");
        }
        if(pid2==0)
        {
            printf("ps starting%d\n ",getpid());
            status_2=execve(args2[0],args2,NULL);
            exit(0);
        }

    }
    return 0;
}
