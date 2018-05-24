#include "ipc.h"

int main(int argc,char *argv[]) {
    int i;
    int rate;
    Msg_buf  msg_arg;

  //�����������е�һ����ָ��һ������˯���������Ե������ִ���ٶ�
    if(argv[1] != NULL)  rate = atoi(argv[1]);
    else rate = 3;   //����һ��Ҫ�����ݵĹ����ڴ�
    buff_key = 101;
    buff_num = STRSIZ+1;
    shm_flg = IPC_CREAT | 0644;
    buff_ptr = (char *)set_shm(buff_key,buff_num,shm_flg);
    //��ϵһ��������Ϣ����
    quest_flg = IPC_CREAT| 0644;
    quest_key = 201;
    quest_id =  set_msq(quest_key,quest_flg);
    //��ϵһ����Ӧ��Ϣ����
    respond_flg = IPC_CREAT| 0644;
    respond_key = 202;
    respond_id =  set_msq(respond_key,respond_flg);
    //ѭ�������
    msg_arg.mid = getpid();
    while(1){
        //����������Ϣ
        msg_arg.mtype = READERQUEST;
        msgsnd(quest_id,&msg_arg,sizeof(msg_arg),0);
        printf("%d reader quest\n",msg_arg.mid);
        //�ȴ��������Ϣ
        msgrcv(respond_id,&msg_arg,sizeof(msg_arg),msg_arg.mid,0);
        printf("%d reading: %s\n",msg_arg.mid,buff_ptr);
        sleep(rate);
        //���������Ϣ
        msg_arg.mtype = FINISHED;
        msgsnd(quest_id,&msg_arg,sizeof(msg_arg),quest_flg);
    }
    return EXIT_SUCCESS;
}
