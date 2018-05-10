#include "ipc.h"

int main(int argc,char *argv[]) {
    int rate;   //�����������е�һ����ָ��һ������˯���������Ե������ִ���ٶ�
    if(argv[1] != NULL)
        rate = atoi(argv[1]);
    else rate = 3;  //��ָ��Ϊ 3 ��

    //�����ڴ� ʹ�õı���
    buff_key = 101;
    //�������θ��ļ�ֵ
    buff_num = 8;
    //�������θ��ĳ���
    cget_key = 103;
    //������ȡ��Ʒָ��ļ�ֵ
    cget_num = 1;
    //ָ����
    shm_flg = IPC_CREAT | 0644;
    //�����ڴ��дȨ��

     //��ȡ������ʹ�õĹ����ڴ棬buff_ptr ָ�򻺳����׵�ַ
     buff_ptr = (char *)set_shm(buff_key,buff_num,shm_flg);
     //��ȡ������ȡ��Ʒָ�룬cget_ptr ָ��������ַ
     cget_ptr = (int *)set_shm(cget_key,cget_num,shm_flg);

    //�ź���ʹ�õı���
    prod_key = 201;  //������ͬ���źŵƼ�ֵ
    pmtx_key = 202;  //�����߻����źŵƼ�ֵ
    cons_key = 301;  //������ͬ���źŵƼ�ֵ
    cmtx_key = 302;  //�����߻����źŵƼ�ֵ
    sem_flg = IPC_CREAT | 0644; //�źŵƲ���Ȩ��

    //������ͬ���źŵƳ�ֵ��Ϊ��������������
    sem_val = buff_num;
    //��ȡ������ͬ���źŵƣ����ñ�ʶ�� prod_sem
    prod_sem = set_sem(prod_key,sem_val,sem_flg);

    //�����߳�ʼ�޲�Ʒ��ȡ��ͬ���źŵƳ�ֵ��Ϊ 0
    sem_val = 0;
    //��ȡ������ͬ���źŵƣ����ñ�ʶ�� cons_sem
    cons_sem = set_sem(cons_key,sem_val,sem_flg);

    //�����߻����źŵƳ�ֵΪ 1
    sem_val = 1;
    //��ȡ�����߻����źŵƣ����ñ�ʶ�� pmtx_sem
    cmtx_sem = set_sem(cmtx_key,sem_val,sem_flg);

     //ѭ��ִ��ģ�������߲���ȡ��Ʒ
     while(1){
        //����޲�Ʒ����������
        down(cons_sem);
        //�����һ����������ȡ��Ʒ��������������
        down(cmtx_sem);

        //�ö�һ�ַ�����ʽģ��������ȡ��Ʒ�����汾���̺ź� ��ȡ���ַ�����ȡ��λ��
        sleep(rate);
        printf("%d consumer get: %c from Buffer[%d]\n",getpid(),buff_ptr[*cget_ptr],*cget_ptr);
        //��ȡλ��ѭ������
        *cget_ptr = (*cget_ptr+1) % buff_num;
        //����������������
        up(cmtx_sem);
        //����������������
        up(prod_sem);
    }

return EXIT_SUCCESS;
}
