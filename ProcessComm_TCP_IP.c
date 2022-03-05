#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int n;
int d;

void childProcess();

int main(int argc, char* argv[]){
	pid_t pid;

	if (argc != 3){
		printf("Insufficient parameters passed.\n");
		exit(1);
	}
	else{
		n = atoi(argv[1]);
		d = atoi(argv[2]);
 
		pid = fork();
		if (pid < 0){
			printf("Fork Error.\n");
			exit(1);
		}
		else if (pid == 0){
			//Child (Producer)
			childProcess();
		}
		else{
			//Parent (Consumer)
			int sockfd, connfd; 
	    	struct sockaddr_in servaddr, cli; 
	  
		    //Socket creation and verification 
		    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
		    if (sockfd == -1) { 
		        printf("Parent socket creation failed.\n");
		        fflush(stdout);
		        exit(1); 
		    } 
		    else{
		        printf("Parent socket successfully created.\n");
			fflush(stdout); 
		    }
		    bzero(&servaddr, sizeof(servaddr)); 
		  
		    //Assign IP, PORT 
		    servaddr.sin_family = AF_INET; 
		    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
		    servaddr.sin_port = htons(PORT); 
	  
		    //Connect the client socket to server socket
		    int conn = 0;
		    while (conn == 0){
			    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
			        sleep(0.1);
			    } 
			    else{
			        printf("Parent connected to the server.\n");
			        fflush(stdout);
			        conn = 1;
			    }
			}

			int count = n;
			int read_val = 0;
			while (count != 0){
				read(sockfd, &read_val, sizeof(read_val));
				printf("Series : %d\n", read_val);
				fflush(stdout);
				count--;
			}
			
			close(sockfd);
		}

	}

	return 0;

}

void childProcess(){
	float r1 = ((float)rand()/RAND_MAX)*(float)(5.0);
	sleep(r1);

	int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    //Socket creation and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("Child socket creation failed.\n");
        fflush(stdout);
       	exit(1); 
    } 
    else{
        printf("Child socket successfully created.\n");
        fflush(stdout);
    }
    bzero(&servaddr, sizeof(servaddr)); 
  
    //Assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    //Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("Child socket bind failed.\n");
        fflush(stdout);
        exit(1);
    }
    else{
        printf("Child socket successfully bound.\n");
        fflush(stdout);
    }
  
    //Server is ready to listen
    if ((listen(sockfd, 5)) != 0) { 
        printf("Child listen failed.\n");
        fflush(stdout);
        exit(1);
    } 
    else{
        printf("Child server listening.\n");
        fflush(stdout);
    }
    len = sizeof(cli); 
  
    //Accept the connection from the client
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("Child server acccept failed.\n");
        fflush(stdout); 
        exit(1);
    } 
    else{
        printf("Child server acccepted the client.\n");
        fflush(stdout);
    }

	int tp = 0;
	float r2;
	for (int k = 0;  k < n; k++){
		tp = k * d;
		write(connfd, &tp, sizeof(tp)); 
		r2 = ((float)rand()/RAND_MAX)*(float)(10.0);
		sleep(r2);
	}

    //Close the socket
    close(sockfd);

}

