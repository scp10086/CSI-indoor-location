/*
 * (c) 2008-2011 Daniel Halperin <dhalperi@cs.washington.edu>
 */
#include "iwl_connector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/netlink.h>

#define MAX_PAYLOAD 2048
#define SLOW_MSG_CNT 1

int sock_fd = -1;						// the socket
FILE* out = NULL;

void check_usage(int argc, char** argv);

FILE* open_file(char* filename, char* spec);

void caught_signal(int sig);

void exit_program(int code);
void exit_program_err(int code, char* func);

int main(int argc, char** argv)
{
	/* Local variables */
	struct sockaddr_nl proc_addr, kern_addr;	// addrs for recv, send, bind
	struct cn_msg *cmsg;
	char buf[4096];
	int ret;
	unsigned short l, l2;
	int count = 0;
int point_count = 1;
char str_point_count[20];
char cell_count = 0;
char input[20];
int input_num;
int max_capture_num = 100;
char address[30];
char head[]= "./first_test/s";
char end[]=".data";
	/* Make sure usage is correct */
	//check_usage(argc, argv);

	


	/* Poll socket forever waiting for a message */
		printf("now the location point number is %d\n",point_count);
		sprintf(str_point_count,"%d",point_count);
		strcpy(address,head);
		strcat(address,str_point_count);
		strcat(address,end);
		printf("now the point data address is %s\n",address);

while(1)
{	
	scanf("%s",input);
	input_num = atoi(input);
	memset(input, 0, sizeof(input));
	if(input_num == 0)
	{
		printf("the data sampling end\n");
		break;
	} 
	if(input_num == 2)
	{
		printf("continue to the next point\n");
		continue;
	}
	if(input_num == 1)
	{
		printf("starting the sampling\n");
		out = open_file(address, "w");
	/* Setup the socket */
	sock_fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
	if (sock_fd == -1)
		exit_program_err(-1, "socket");

	/* Initialize the address structs */
	memset(&proc_addr, 0, sizeof(struct sockaddr_nl));
	proc_addr.nl_family = AF_NETLINK;
	proc_addr.nl_pid = getpid();			// this process' PID
	proc_addr.nl_groups = CN_IDX_IWLAGN;
	memset(&kern_addr, 0, sizeof(struct sockaddr_nl));
	kern_addr.nl_family = AF_NETLINK;
	kern_addr.nl_pid = 0;					// kernel
	kern_addr.nl_groups = CN_IDX_IWLAGN;

	/* Now bind the socket */
	if (bind(sock_fd, (struct sockaddr *)&proc_addr, sizeof(struct sockaddr_nl)) == -1)
		exit_program_err(-1, "bind");

	/* And subscribe to netlink group */
	{
		int on = proc_addr.nl_groups;
		ret = setsockopt(sock_fd, 270, NETLINK_ADD_MEMBERSHIP, &on, sizeof(on));
		if (ret)
			exit_program_err(-1, "setsockopt");
	}

	/* Set up the "caught_signal" function as this program's sig handler */
	signal(SIGINT, caught_signal);
	while (1)
	{


		
		/* Open and check log file */
		
		/* Receive from socket with infinite timeout */
		ret = recv(sock_fd, buf, sizeof(buf), 0);
		if (ret == -1)
			exit_program_err(-1, "recv");
		/* Pull out the message portion and print some stats */
		cmsg = NLMSG_DATA(buf);


		if (count % SLOW_MSG_CNT == 0)
			printf("received %d bytes: id: %d val: %d seq: %d clen: %d count %d\n", cmsg->len, cmsg->id.idx, cmsg->id.val, cmsg->seq, cmsg->len, count);
		/* Log the data to file */
		l = (unsigned short) cmsg->len;
		l2 = htons(l);
		fwrite(&l2, 1, sizeof(unsigned short), out);
		ret = fwrite(cmsg->data, 1, l, out);

		if (count == max_capture_num)
		{
			printf("wrote %d bytes [msgcnt=%u]\n", ret, count);
			break;
		}		
		count++;
		if (ret != l)
			exit_program_err(1, "fwrite");

		

	}
		count=0;
		exit_program(0);
		point_count++;
		printf("now the location point number is %d\n",point_count);
		memset(address, 0, sizeof(address)); 
		memset(str_point_count, 0, sizeof(str_point_count)); 
		sprintf(str_point_count,"%d",point_count);
		strcpy(address,head);
		strcat(address,str_point_count);
		strcat(address,end);
		printf("now the point data address is %s\n",address);
	}
}
	
	printf("end the sampling\n");
	
	return 0;
}

void check_usage(int argc, char** argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <output_file>\n", argv[0]);
		exit_program(1);
	}
}

FILE* open_file(char* filename, char* spec)
{
	FILE* fp = fopen(filename, spec);
	if (!fp)
	{
		perror("fopen");
		exit_program(1);
	}
	return fp;
}

void caught_signal(int sig)
{
	fprintf(stderr, "Caught signal %d\n", sig);
	exit_program(0);
}

void exit_program(int code)
{
	if (out)
	{
		fclose(out);
		out = NULL;
	}
	if (sock_fd != -1)
	{
		close(sock_fd);
		sock_fd = -1;
	}
	//exit(code);
}

void exit_program_err(int code, char* func)
{
	perror(func);
	exit_program(code);
}
