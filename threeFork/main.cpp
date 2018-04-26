#include<stdio.h>
#include<unistd.h>
int main()
{
    pid_t pid[3];
    int count = 0;
    pid[0] = fork();
    pid[1] = fork();
    pid[2] = fork();

    printf("this is process\n");

    return 0;
}
