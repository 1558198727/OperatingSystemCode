#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>

#define BUFSZ   256    //建立或获取 ipc 的一组函数的原型说明

int get_ipc_id(char *proc_file,key_t key);
char *set_shm(key_t shm_key,int shm_num,int shm_flag);
int set_msq(key_t msq_key,int msq_flag);
int set_sem(key_t sem_key,int sem_val,int sem_flag);
int down(int sem_id);
int up(int sem_id);

/*信号灯控制用的共同体*/
typedef union semuns {
    int val;
} Sem_uns;

/* 消息结构体*/
typedef struct msgbuf {
    long mtype;
    char mtext[1];
} Msg_buf;

//生产消费者共享缓冲区即其有关的变量
key_t buff_key;  int buff_num;  char *buff_ptr;

//生产者放产品位置的共享指针
key_t pput_key;  int pput_num;
int *pput_ptr;

//消费者取产品位置的共享指针
key_t cget_key;  int cget_num;
int *cget_ptr;

//生产者有关的信号量
key_t prod_key;
key_t pmtx_key;
int prod_sem;
int pmtx_sem;

//消费者有关的信号量
key_t cons_key;
key_t cmtx_key;
int cons_sem;
int cmtx_sem;
int sem_val;
int sem_flg;
int shm_flg;


 #include "ipc.h"
int get_ipc_id(char *proc_file,key_t key) {
    FILE *pf;
    int i,j;
    char line[BUFSZ],colum[BUFSZ];
    if((pf = fopen(proc_file,"r")) == NULL){
        perror("Proc file not open");
        exit(EXIT_FAILURE);
    }
    fgets(line, BUFSZ,pf);
    while(!feof(pf)){
        i = j = 0;
        fgets(line, BUFSZ,pf);
        while(line[i] == ' ') i++;
        while(line[i] !=' ')
            colum[j++] = line[i++];
            colum[j] = '\0';
        if(atoi(colum) != key)
            continue;
        j=0;
        while(line[i] == ' ')
            i++;
        while(line[i] !=' ')
            colum[j++] = line[i++];
        colum[j] = '\0';
        i = atoi(colum);
        fclose(pf);
        return i;
    }
    fclose(pf);
    return -1;
}
int down(int sem_id) {
    struct sembuf buf;
    buf.sem_op = -1;
    buf.sem_num = 0;
    buf.sem_flg = SEM_UNDO;
    if((semop(sem_id,&buf,1)) <0) {
        perror("down error ");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int up(int sem_id) {
    struct sembuf buf;
    buf.sem_op = 1;
    buf.sem_num = 0;
    buf.sem_flg = SEM_UNDO;
    if((semop(sem_id,&buf,1)) <0) {
        perror("up error ");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int set_sem(key_t sem_key,int sem_val,int sem_flg) {
    int sem_id;  Sem_uns sem_arg;

    //测试由 sem_key 标识的信号灯数组是否已经建立
    if((sem_id = get_ipc_id("/proc/sysvipc/sem",sem_key)) < 0 ){
        //semget 新建一个信号灯,其标号返回到 sem_id
        if((sem_id = semget(sem_key,1,sem_flg)) < 0){
            perror("semaphore create error");
            exit(EXIT_FAILURE);
        }
        //设置信号灯的初值
        sem_arg.val = sem_val;
        if(semctl(sem_id,0,SETVAL,sem_arg) <0){
            perror("semaphore set error");
            exit(EXIT_FAILURE);
        }
    }
   return sem_id;
}

char * set_shm(key_t shm_key,int shm_num,int shm_flg) {
    int i,shm_id;
    char * shm_buf;

    //测试由 shm_key 标识的共享内存区是否已经建立
    if((shm_id = get_ipc_id("/proc/sysvipc/shm",shm_key)) < 0 ){
        //shmget 新建 一个长度为 shm_num 字节的共享内存,其标号返回到 shm_id
        if((shm_id = shmget(shm_key,shm_num,shm_flg)) <0){
            perror("shareMemory set error");
        exit(EXIT_FAILURE);
    }
    //shmat 将由 shm_id 标识的共享内存附加给指针 shm_buf
    if((shm_buf = (char *)shmat(shm_id,0,0)) < (char *)0){
        perror("get shareMemory error");
        exit(EXIT_FAILURE);
    }
    for(i=0; i<shm_num; i++) shm_buf[i] = 0;
        //初始为 0
    }
    //shm_key 标识的共享内存区已经建立,将由 shm_id 标识的共享内存附加给指 针 shm_buf
    if((shm_buf = (char *)shmat(shm_id,0,0)) < (char *)0){
        perror("get shareMemory error");
        exit(EXIT_FAILURE);
    }
return shm_buf;
}

int set_msq(key_t msq_key,int msq_flg){
    int msq_id;
    //测试由 msq_key 标识的消息队列是否已经建立
    if((msq_id = get_ipc_id("/proc/sysvipc/msg",msq_key)) < 0 ){
            //msgget 新建一个消息队列,其标号返回到 msq_id
        if((msq_id = msgget(msq_key,msq_flg)) < 0){
            perror("messageQueue set error");
        exit(EXIT_FAILURE);
        }
    }
    return msq_id;
}

3) 在当前新文件夹中建立生产者程序 producer.c


#include "ipc.h"

int main(int argc,char *argv[]) {
    int rate;
    //可在在命令行第一参数指定一个进程睡眠秒数，以调解进程执行速度
    if(argv[1] != NULL)  rate = atoi(argv[1]);
    else rate = 3;  //不指定为 3 秒

  //共享内存使用的变量

    buff_key = 101;//缓冲区任给的键值
    buff_num = 8;//缓冲区任给的长度
    pput_key = 102;//生产者放产品指针的键值
    pput_num = 1; //指针数
    shm_flg = IPC_CREAT | 0644;//共享内存读写权限

    //获取缓冲区使用的共享内存，buff_ptr 指向缓冲区首地址
    buff_ptr = (char *)set_shm(buff_key,buff_num,shm_flg);
    //获取生产者放产品位置指针 pput_ptr
    pput_ptr = (int *)set_shm(pput_key,pput_num,shm_flg);

    //信号量使用的变量
    prod_key = 201;//生产者同步信号灯键值
    pmtx_key = 202;//生产者互斥信号灯键值
    cons_key = 301;//消费者同步信号灯键值
    cmtx_key = 302;//消费者互斥信号灯键值
    sem_flg = IPC_CREAT | 0644;

    //生产者同步信号灯初值设为缓冲区最大可用量
    sem_val = buff_num;     //获取生产者同步信号灯，引用标识存 prod_sem
    prod_sem = set_sem(prod_key,sem_val,sem_flg);

    //消费者初始无产品可取，同步信号灯初值设为 0
    sem_val = 0;
    //获取消费者同步信号灯，引用标识存 cons_sem
    cons_sem = set_sem(cons_key,sem_val,sem_flg);

    //生产者互斥信号灯初值为 1
    sem_val = 1;     //获取生产者互斥信号灯，引用标识存 pmtx_sem
    pmtx_sem = set_sem(pmtx_key,sem_val,sem_flg);
    //循环执行模拟生产者不断放产品
    while(1){
        //如果缓冲区满则生产者阻塞
        down(prod_sem);
        //如果另一生产者正在放产品，本生产者阻塞
        down(pmtx_sem);
        //用写一字符的形式模拟生产者放产品，报告本进程号和放入的字符及存 放的位置
        buff_ptr[*pput_ptr] = 'A'+ *pput_ptr;
        sleep(rate);
        printf("%d producer put: %c to Buffer[%d]\n",getpid(),buff_ptr[*pput_ptr],*pput_ptr);

         //存放位置循环下移
        *pput_ptr = (*pput_ptr+1) % buff_num;

         //唤醒阻塞的生产者
         up(pmtx_sem);
         //唤醒阻塞的消费者
         up(cons_sem);
     }
return EXIT_SUCCESS;
}

4) 在当前新文件夹中建立消费者程序 consumer.c

#include "ipc.h"

int main(int argc,char *argv[]) {
    int rate;   //可在在命令行第一参数指定一个进程睡眠秒数，以调解进程执行速度
    if(argv[1] != NULL)
        rate = atoi(argv[1]);
    else rate = 3;  //不指定为 3 秒

    //共享内存 使用的变量
    buff_key = 101;
    //缓冲区任给的键值
    buff_num = 8;
    //缓冲区任给的长度
    cget_key = 103;
    //消费者取产品指针的键值
    cget_num = 1;
    //指针数
    shm_flg = IPC_CREAT | 0644;
    //共享内存读写权限

     //获取缓冲区使用的共享内存，buff_ptr 指向缓冲区首地址
     buff_ptr = (char *)set_shm(buff_key,buff_num,shm_flg);
     //获取消费者取产品指针，cget_ptr 指向索引地址
     cget_ptr = (int *)set_shm(cget_key,cget_num,shm_flg);

    //信号量使用的变量
    prod_key = 201;  //生产者同步信号灯键值
    pmtx_key = 202;  //生产者互斥信号灯键值
    cons_key = 301;  //消费者同步信号灯键值
    cmtx_key = 302;  //消费者互斥信号灯键值
    sem_flg = IPC_CREAT | 0644; //信号灯操作权限

    //生产者同步信号灯初值设为缓冲区最大可用量
    sem_val = buff_num;
    //获取生产者同步信号灯，引用标识存 prod_sem
    prod_sem = set_sem(prod_key,sem_val,sem_flg);

    //消费者初始无产品可取，同步信号灯初值设为 0
    sem_val = 0;
    //获取消费者同步信号灯，引用标识存 cons_sem
    cons_sem = set_sem(cons_key,sem_val,sem_flg);

    //消费者互斥信号灯初值为 1
    sem_val = 1;
    //获取消费者互斥信号灯，引用标识存 pmtx_sem
    cmtx_sem = set_sem(cmtx_key,sem_val,sem_flg);

     //循环执行模拟消费者不断取产品
     while(1){
        //如果无产品消费者阻塞
        down(cons_sem);
        //如果另一消费者正在取产品，本消费者阻塞
        down(cmtx_sem);

        //用读一字符的形式模拟消费者取产品，报告本进程号和 获取的字符及读取的位置
        sleep(rate);
        printf("%d consumer get: %c from Buffer[%d]\n",getpid(),buff_ptr[*cget_ptr],*cget_ptr);
        //读取位置循环下移
        *cget_ptr = (*cget_ptr+1) % buff_num;
        //唤醒阻塞的消费者
        up(cmtx_sem);
        //唤醒阻塞的生产者
        up(prod_sem);
    }

return EXIT_SUCCESS;
}
