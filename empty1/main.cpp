
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sched.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/resource.h>
using namespace std;
int main(int argc, char *argv[])  {
    int i,j,status;
    int pid[3];
    struct sched_param p[3];
    cout<<"pid "<<(pid[0]=fork())<<endl;
    cout<<"pid "<<(pid[1]=fork())<<endl;
    cout<<"pid "<<(pid[2]=fork())<<endl;
        //if((pid[i]=fork()) >0){;

        //} else{

         //       printf("Child PID = %d \n",getpid());
         //       sleep(1);
        //
         //   exit( EXIT_SUCCESS);
       // }
cout<<"pid "<<endl;

    return EXIT_SUCCESS;
}
