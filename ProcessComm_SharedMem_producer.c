#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUF_SIZE 8
#define SM_KEY 0x1234

struct smseg {
        int input;
        int out;
        int buf[BUF_SIZE];
	int finish;
};

int n = 1;
int d = 1;

int main(int argc, char* argv[]){
	int smPrId;
	struct smseg *smPrPtr;

	if (argc != 3){
		printf("Insufficient parameters passed.\n");
		return 1;
	}
	else{
		n = atoi(argv[1]);
		d = atoi(argv[2]);
		printf("Address of n from running producer : %p\n", &n);
		smPrId = shmget(SM_KEY, sizeof(struct smseg), 0644|IPC_CREAT);
		if (smPrId < 0){
			printf("shmget Error.\n");
			return 1;
		}

		smPrPtr = shmat(smPrId, NULL, 0);
		if (smPrPtr == (void*) -1){
			printf("shmat Error.\n");
			return 1;
		}
		printf("Producer can access the shared memory.\n");
		printf("Shared memory address for producer : %p\n", smPrPtr);
		int tp = 0;
		float r;
		for (int k = 0;  k < n; k++){
			tp = k * d;
			while (((smPrPtr->input + 1) % BUF_SIZE) == smPrPtr->out);
			smPrPtr->buf[smPrPtr->input] = tp;
			smPrPtr->input = (smPrPtr->input + 1) % BUF_SIZE;
			r = ((float)rand()/RAND_MAX)*(float)(10.0);
			sleep(r);
		}
		smPrPtr->finish = 1;
	}

	return 0;
}


