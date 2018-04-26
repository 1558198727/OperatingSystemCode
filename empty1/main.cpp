#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>

#define BUFSZ   256    //�������ȡ ipc ��һ�麯����ԭ��˵��

int get_ipc_id(char *proc_file,key_t key);
char *set_shm(key_t shm_key,int shm_num,int shm_flag);
int set_msq(key_t msq_key,int msq_flag);
int set_sem(key_t sem_key,int sem_val,int sem_flag); int down(int sem_id);
int up(int sem_id);

/*�źŵƿ����õĹ�ͬ��*/
typedef union semuns {
    int val;
} Sem_uns;

/* ��Ϣ�ṹ��*/
typedef struct msgbuf {
    long mtype;
    char mtext[1];
} Msg_buf;

//���������߹������������йصı���
key_t buff_key;  int buff_num;  char *buff_ptr;

//�����߷Ų�Ʒλ�õĹ���ָ��
key_t pput_key;  int pput_num;
int *pput_ptr;

//������ȡ��Ʒλ�õĹ���ָ��
key_t cget_key;  int cget_num;
int *cget_ptr;

//�������йص��ź���
key_t prod_key;
key_t pmtx_key;
int prod_sem;
int pmtx_sem;

//�������йص��ź���
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
