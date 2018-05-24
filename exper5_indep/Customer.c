#include "ipc.h"
int main(int argc,char *argv[])
{
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

    int sofa_count=0;
    int wait_count=0;
    int i=0;
 //   int count=0;
    while(1) {
        sleep(rate);
    //    count++;
    //    printf("count = %d ", count);
        i++;
     //   printf("i = %d ", i);
        msg_arg.mid = i;
        if(sofa_count < 4) {
            if(wait_count != 0) {
                i--;
                //������ʽ������Ϣ
                msgrcv(wait_quest_id, &msg_arg, sizeof(msg_arg), 0, 0);
                 printf("mid = %d ", msg_arg.mid);
                msgsnd(wait_respond_id, &msg_arg,sizeof(msg_arg), 0);
                printf("%d customer from waiting room to sofa\n", msg_arg.mid);
             //   up(costomer_sem);
            } else {
                printf("%d new customer sit sofa\n", i);
            }

            sofa_quest_flg=IPC_NOWAIT;
            if(msgsnd(sofa_quest_id, &msg_arg, sizeof(msg_arg), sofa_quest_flg) >= 0){
              //  sofa_count++;
             //  return 0;
            }
            sofa_count++;

        } else if(wait_count < 13) {
            printf("sofa is full ��%d customer is waiting in the waiting room\n", i);
            wait_quest_flg = IPC_NOWAIT;
            msgsnd(wait_quest_id, &msg_arg, sizeof(msg_arg), wait_quest_flg);
            wait_count++;

        } else {
            printf("waiting room is full ��%d customer can't get into barber\n", i);
         //   down(costomer_sem);
              msgrcv(sofa_respond_id, &msg_arg, sizeof(msg_arg), 0, 0);
              sofa_count--;
              i--;
        }
        sofa_quest_flg=IPC_NOWAIT;
        if(msgrcv(sofa_respond_id, &msg_arg, sizeof(msg_arg), 0, sofa_quest_flg)>=0){
            sofa_count--;
        }
        wait_quest_flg = IPC_NOWAIT;
        if(msgrcv(wait_respond_id, &msg_arg, sizeof(msg_arg), 0, wait_quest_flg)>=0) {
            wait_count--;
        }
    }

    return 0;
}
