#include "ipc.h"
int main(int argc,char *argv[])
{
  //  int i;
    int rate;
    Msg_buf msg_arg;
    //�����������е�һ����ָ��һ������˯������,�Ե������ִ���ٶ�
    if(argv[1] != NULL) rate = atoi(argv[1]);
    else rate = 3;

    //��ϵһ��������Ϣ����
    wait_quest_flg = IPC_CREAT| 0644;
    wait_quest_key = 101;
    wait_quest_id = set_msq(wait_quest_key,wait_quest_flg);
    //��ϵһ����Ӧ��Ϣ����
    wait_respond_flg = IPC_CREAT| 0644;
    wait_respond_key = 102;
    wait_respond_id = set_msq(wait_respond_key,wait_respond_flg);

    //��ϵһ��������Ϣ����
    sofa_quest_flg = IPC_CREAT| 0644;
    sofa_quest_key = 201;
    sofa_quest_id = set_msq(sofa_quest_key,sofa_quest_flg);
    //��ϵһ����Ӧ��Ϣ����
    sofa_respond_flg = IPC_CREAT| 0644;
    sofa_respond_key = 202;
    sofa_respond_id = set_msq(sofa_respond_key,sofa_respond_flg);

    //�ź���ʹ�õı���
    costomer_key = 301;//�˿�ͬ���źŵƼ�ֵ
    account_key = 302;//�˲������źŵƼ�ֵ
    sem_flg = IPC_CREAT | 0644;
    //�˿�ͬ���źŵƳ�ֵ��Ϊ0
    sem_val = 0;
    //��ȡ�˿�ͬ���źŵ�,���ñ�ʶ�� costomer_sem
    costomer_sem = set_sem(costomer_key,sem_val,sem_flg);
    //�˲������źŵƳ�ֵ��Ϊ 1
    sem_val = 1;
    //��ȡ������ͬ���źŵ�,���ñ�ʶ�� cons_sem
    account_sem = set_sem(account_key,sem_val,sem_flg);

    int pid1, pid2;
    pid1=fork();
    if(pid1==0) {
        while(1) {
           //  wait_quest_flg=IPC_NOWAIT;
            printf("%d barber is sleeping \n", getpid());
            wait_quest_flg=0;
            if(msgrcv(sofa_quest_id, &msg_arg, sizeof(msg_arg), 0, wait_quest_flg)>=0) {
                msgsnd(sofa_respond_id, &msg_arg,sizeof(msg_arg), 0);
                printf("%d barber is serving for %d customer \n", getpid(), msg_arg.mid);
                sleep(rate);
                down(account_sem);
                printf("%d barber is collect %d customer's money\n", getpid(), msg_arg.mid);
                up(account_sem);
           }
        }
    } else {
        pid2=fork();
        if(pid2==0) {
            while(1) {
               //  wait_quest_flg=IPC_NOWAIT;
               printf("%d barber is sleeping\n", getpid());
               wait_quest_flg=0;
               if(msgrcv(sofa_quest_id, &msg_arg, sizeof(msg_arg), 0, wait_quest_flg)>=0) {
                    msgsnd(sofa_respond_id, &msg_arg,sizeof(msg_arg), 0);
                    printf("%d barber is serving for %d customer \n", getpid(), msg_arg.mid);
                    sleep(rate);
                    down(account_sem);
                    printf("%d barber is collect %d customer's money\n", getpid(), msg_arg.mid);
                    up(account_sem);
               } else {
                    printf("%d barber is sleeping\n", getpid());

               }
            }
        } else {
             while(1) {
             //  wait_quest_flg=IPC_NOWAIT;
               printf("%d barber is sleeping\n", getpid());
               wait_quest_flg=0;
               if(msgrcv(sofa_quest_id, &msg_arg, sizeof(msg_arg), 0, wait_quest_flg)>=0) {
                    msgsnd(sofa_respond_id, &msg_arg,sizeof(msg_arg), 0);
                    printf("%d barber is serving for %d customer \n", getpid(), msg_arg.mid);
                    sleep(rate);
                    down(account_sem);
                    printf("%d barber is collect %d customer's money\n", getpid(), msg_arg.mid);
                    up(account_sem);
               } else {
                    printf("%d barber is sleeping\n", getpid());

               }
            }
        }
    }
    return 0;
}
