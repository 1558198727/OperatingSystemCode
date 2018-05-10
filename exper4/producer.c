#include "ipc.h"

int main(int argc,char *argv[]) {
    int rate;
    //�����������е�һ����ָ��һ������˯���������Ե������ִ���ٶ�
    if(argv[1] != NULL)  rate = atoi(argv[1]);
    else rate = 3;  //��ָ��Ϊ 3 ��

  //�����ڴ�ʹ�õı���

    buff_key = 101;//�������θ��ļ�ֵ
    buff_num = 8;//�������θ��ĳ���
    pput_key = 102;//�����߷Ų�Ʒָ��ļ�ֵ
    pput_num = 1; //ָ����
    shm_flg = IPC_CREAT | 0644;//�����ڴ��дȨ��

    //��ȡ������ʹ�õĹ����ڴ棬buff_ptr ָ�򻺳����׵�ַ
    buff_ptr = (char *)set_shm(buff_key,buff_num,shm_flg);
    //��ȡ�����߷Ų�Ʒλ��ָ�� pput_ptr
    pput_ptr = (int *)set_shm(pput_key,pput_num,shm_flg);

    //�ź���ʹ�õı���
    prod_key = 201;//������ͬ���źŵƼ�ֵ
    pmtx_key = 202;//�����߻����źŵƼ�ֵ
    cons_key = 301;//������ͬ���źŵƼ�ֵ
    cmtx_key = 302;//�����߻����źŵƼ�ֵ
    sem_flg = IPC_CREAT | 0644;

    //������ͬ���źŵƳ�ֵ��Ϊ��������������
    sem_val = buff_num;     //��ȡ������ͬ���źŵƣ����ñ�ʶ�� prod_sem
    prod_sem = set_sem(prod_key,sem_val,sem_flg);

    //�����߳�ʼ�޲�Ʒ��ȡ��ͬ���źŵƳ�ֵ��Ϊ 0
    sem_val = 0;
    //��ȡ������ͬ���źŵƣ����ñ�ʶ�� cons_sem
    cons_sem = set_sem(cons_key,sem_val,sem_flg);

    //�����߻����źŵƳ�ֵΪ 1
    sem_val = 1;     //��ȡ�����߻����źŵƣ����ñ�ʶ�� pmtx_sem
    pmtx_sem = set_sem(pmtx_key,sem_val,sem_flg);
    //ѭ��ִ��ģ�������߲��ϷŲ�Ʒ
    while(1){
        //�����������������������
        down(prod_sem);
        //�����һ���������ڷŲ�Ʒ��������������
        down(pmtx_sem);
        //��дһ�ַ�����ʽģ�������߷Ų�Ʒ�����汾���̺źͷ�����ַ����� �ŵ�λ��
        buff_ptr[*pput_ptr] = 'A'+ *pput_ptr;
        sleep(rate);
        printf("%d producer put: %c to Buffer[%d]\n",getpid(),buff_ptr[*pput_ptr],*pput_ptr);

         //���λ��ѭ������
        *pput_ptr = (*pput_ptr+1) % buff_num;

         //����������������
         up(pmtx_sem);
         //����������������
         up(cons_sem);

     }
return EXIT_SUCCESS;
}
