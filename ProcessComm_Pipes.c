#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1

int n;
int d;

int main(int argc, char* argv[])
{
	int read_val;
	int fd[2];
	pid_t pid;
	
	if (argc != 3){
		printf("Insufficient parameters passed.\n");
		return -1;
	}
	
	n = atoi(argv[1]);
	d = atoi(argv[2]);

	/* Create the pipe */
	if (pipe(fd) == -1) {
		printf("Pipe creation failed.\n");
		return -1;
	}

	/* Fork a child process */
	pid = fork();
	if(pid<0){ /* Error occured */
		printf("Fork failed.\n");
		return -1;
	}
	else if (pid > 0) {/* Parent process* /
				/* Close the write end since we are the consumer */
		close(fd[WRITE_END]);
		
		while (n > 0){
			/* Read from the pipe and print the message to screen */
			read(fd[READ_END], &read_val, sizeof(read_val));
			printf("Series %d\n", read_val);
			n--;
			fflush(stdout);
		}
		
		if(read(fd[READ_END], &read_val, sizeof(read_val)) < 0){
			printf("Read error.\n");
			fflush(stdout);
		}
		else{
			/* Close the read end now that we are done */
			printf("Operation done.\n");
			fflush(stdout);
			close(fd[READ_END]);
		}
	}
	else { /* Child process*/
		
		/* Close the read end since we are the producer */
		close(fd[READ_END]);
		/* Write to the pipe */
		int tp;
		float r;
		for (int k = 0;  k < n; k++){
			tp = k * d;
			write(fd[WRITE_END], &tp, sizeof(tp));
			/* Sleep */
			r = ((float)rand()/RAND_MAX)*(float)(10.0);			
			sleep(r);
		}
		/* Close the write end, now that we are done */
		close(fd[WRITE_END]);
	}
	
	return 0;
}
