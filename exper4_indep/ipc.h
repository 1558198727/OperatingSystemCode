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
int set_sem(key_t sem_key,int sem_val,int sem_flag);
int down(int sem_id);
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

//���������߹��������������йصı���
key_t buff_key;  int buff_num;  char *buff_ptr;

//�����߷Ų�Ʒλ�õĹ���ָ��
key_t pput_key;  int pput_num;
int *pput_ptr;

//������ȡ��Ʒλ�õĹ���ָ��
key_t cget_key;
int cget_num;
int *cget_ptr;
int buff_num;
int pput_num;
int buff_number;
int pput_number;
int cget_number;

key_t pput_h;
key_t prod_h;
key_t pmtx_h;
key_t cons_h;
key_t cmtx_h;

key_t buff_h;
key_t cget_h;

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