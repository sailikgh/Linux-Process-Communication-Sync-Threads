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
	int smCId;
	struct smseg *smCPtr;

	if (argc != 3){
		printf("Insufficient parameters passed.\n");
		return 1;
	}
	else{
		n = atoi(argv[1]);
		d = atoi(argv[2]);
		printf("Address of n from running consumer : %p\n", &n);
		smCId = shmget(SM_KEY, sizeof(struct smseg), 0644|IPC_CREAT);
		if (smCId < 0){
			printf("shmget Error.\n");
			return 1;
		}

		smCPtr = shmat(smCId, NULL, 0);
		if (smCPtr == (void*) -1){
			printf("shmat Error.\n");
			return 1;
		}
		printf("Consumer has attached the shared memory.\n");
		printf("Shared memory address for consumer : %p\n", smCPtr);
		smCPtr->input = 0;
		smCPtr->out = 0;
		smCPtr->finish = 0;

		int counter = n;

		while (counter != 0){
			if (smCPtr->input != smCPtr->out){
				printf("Series : %d\n", smCPtr->buf[smCPtr->out]);
				fflush(stdout);
				smCPtr->out = (smCPtr->out + 1) % BUF_SIZE;
				counter--;
			}
		}

	}

	return 0;
}
