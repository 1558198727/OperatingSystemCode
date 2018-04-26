#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
typedef struct{
    int value;
    short sem_op;
    //short sem_flg;
}semaphore;
bool SemWait(semaphore* sem){
    sem->value--;
    if(sem->value < 0){
        pause();
    }
}

bool SemSignal(semaphore* sem){
    sem->value++;
    //if(sem->value <= 0){
        //pause();
    //}
}

semaphore sem;

void initSem(semaphore* sem){
    printf("init the sem\n");
    sem->value = 0;
}
int main()
{
    initSem(&sem);
    pid_t child[3];
    child[0] = fork();

    if(child[0] < 0)//调用fork失败
    {
        perror("the fork failed");
        exit(1);
    }
    else if(child[0] == 0)//子进程
    {
        printf("我是第child process0，PID = %d\n",getpid());

    }else{
        child[1] = fork();

        if(child[1] < 0)//调用fork失败
        {
            perror("the fork failed");
            exit(1);
        }
        else if(child[1] == 0)//子进程
        {
            printf("我是第child process1，PID = %d\n",getpid());

        }else{
            child[2] = fork();

            if(child[2] < 0)//调用fork失败
            {
                perror("the fork failed");
                exit(1);
            }
            else if(child[2] == 0)//子进程
            {
                printf("I am child process2，PID = %d\n",getpid());

            }else{


            }

        }
    }

    /*
    pid = fork();
    if(pid < 0){
        printf("create child process failed\n");
        exit(-1);
    }else if(pid == 0){//children process
        execlp("/bin/ls","ls",NULL);
    }else{//parents process
        wait(NULL);
        printf("child process completed\n");
        printf("Hello world!\n");
        exit(0);
    }*/

    return 0;
}
