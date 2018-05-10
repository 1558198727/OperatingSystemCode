 #include "ipc.h"

int get_ipc_id(char *proc_file,h_t h) {
	FILE *pf;
	int m,n;
	char line[BUFSZ],colum[BUFSZ];
	if((pf = fopen(proc_file,"r")) == NULL){
		perror("Proc file not open");
		exit(EXIT_FAILURE);
	}
	fgets(line, BUFSZ,pf);
	while(!feof(pf)){
		m = n = 0;
		fgets(line, BUFSZ,pf);
		while(line[m] == ' ')
			m++;
		while(line[m] !=' ')
			colum[n++] = line[m++];
		colum[n] = '\0';
		if(atoi(colum) != h)
			continue;
		n=0;
		while(line[m] == ' ')
			m++;
		while(line[m] !=' ')
			colum[n++] = line[m++];
		colum[n] = '\0';
		m = atoi(colum);
		fclose(pf);
		return m;
	}
	fclose(pf);
	return -1;
}

int down(int sem_id) {
	struct sembuf buf;
	buf.sem_op = -1;
	buf.sem_number = 0;
	buf.sem_flg = SEM_UNDO;
	if((semop(sem_id,&buf,1)) <0) {
	    perror("down error ");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}


int up(int sem_id){
	struct sembuf buf;
	buf.sem_op = 1;
	buf.sem_number = 0;
	buf.sem_flg = SEM_UNDO;
	if((semop(sem_id,&buf,1)) <0) {
		perror("up error ");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}


int set_sem(h_t sem_h,int sem_val,int sem_flg) {
	int sem_id;
	Sem_uns sem_arg;
	if((sem_id = get_ipc_id("/proc/sysvipc/sem",sem_h)) < 0 ) {
		if((sem_id = semget(sem_h,1,sem_flg)) < 0) {
			perror("semaphore create error");
			exit(EXIT_FAILURE);
		}
				sem_arg.val = sem_val;
		if(semctl(sem_id,0,SETVAL,sem_arg) <0) {
			perror("semaphore set error");
			exit(EXIT_FAILURE);
		}
	}
	return sem_id;
}

char * set_shm(h_t shm_h,int shm_number,int shm_flg) {
	int m,shm_id;
	char * shm_buf;
	if((shm_id = get_ipc_id("/proc/sysvipc/shm",shm_h)) < 0 ) {
		if((shm_id = shmget(shm_h,shm_number,shm_flg)) <0){
			perror("shareMemory set error");
			exit(EXIT_FAILURE);
		}
		if((shm_buf = (char *)shmat(shm_id,0,0)) < (char *)0) {
			perror("get shareMemory error");
			exit(EXIT_FAILURE);
		}
		for(m=0; m<shm_number; m++)
			shm_buf[m] = 0;
	}
	if((shm_buf = (char *)shmat(shm_id,0,0)) < (char *)0){
		perror("get shareMemory error");
		exit(EXIT_FAILURE);
	}
	return shm_buf;
}

int set_msq(h_t msq_h,int msq_flg) {
	int msq_id;
	if((msq_id = get_ipc_id("/proc/sysvipc/msg",msq_h)) < 0 ) {
		if((msq_id = msgget(msq_h,msq_flg)) < 0){
			perror("messageQueue set error");
			exit(EXIT_FAILURE);
		}
	}
	return msq_id;
}
