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

    //������ sem_key ��ʶ���źŵ������Ƿ��Ѿ�����
    if((sem_id = get_ipc_id("/proc/sysvipc/sem",sem_key)) < 0 ){
        //semget �½�һ���źŵ�,���ŷ��ص� sem_id
        if((sem_id = semget(sem_key,1,sem_flg)) < 0){
            perror("semaphore create error");
            exit(EXIT_FAILURE);
        }
        //�����źŵƵĳ�ֵ
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

    //������ shm_key ��ʶ�Ĺ����ڴ����Ƿ��Ѿ�����
    if((shm_id = get_ipc_id("/proc/sysvipc/shm",shm_key)) < 0 ){
        //shmget �½� һ������Ϊ shm_num �ֽڵĹ����ڴ�,���ŷ��ص� shm_id
        if((shm_id = shmget(shm_key,shm_num,shm_flg)) <0){
            perror("shareMemory set error");
        exit(EXIT_FAILURE);
    }
    //shmat ���� shm_id ��ʶ�Ĺ����ڴ渽�Ӹ�ָ�� shm_buf
    if((shm_buf = (char *)shmat(shm_id,0,0)) < (char *)0){
        perror("get shareMemory error");
        exit(EXIT_FAILURE);
    }
    for(i=0; i<shm_num; i++) shm_buf[i] = 0;
        //��ʼΪ 0
    }
    //shm_key ��ʶ�Ĺ����ڴ����Ѿ�����,���� shm_id ��ʶ�Ĺ����ڴ渽�Ӹ�ָ �� shm_buf
    if((shm_buf = (char *)shmat(shm_id,0,0)) < (char *)0){
        perror("get shareMemory error");
        exit(EXIT_FAILURE);
    }
return shm_buf;
}

int set_msq(key_t msq_key,int msq_flg){
    int msq_id;
    //������ msq_key ��ʶ����Ϣ�����Ƿ��Ѿ�����
    if((msq_id = get_ipc_id("/proc/sysvipc/msg",msq_key)) < 0 ){
            //msgget �½�һ����Ϣ����,���ŷ��ص� msq_id
        if((msq_id = msgget(msq_key,msq_flg)) < 0){
            perror("messageQueue set error");
        exit(EXIT_FAILURE);
        }
    }
    return msq_id;
}