// writer process

#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>


//struct for message queue
struct mesg_buffer {
	long mesg_type;
	char mesg_text[10000];
} message;

int main(char* name)
{
	//gcc sender.c -o sender
	// ./sender
	key_t key;
	int msgid;

	// ftok to generate unique key
	key = ftok("message_queue_name", 'B');
	// printf("key is %d \n", key);

	//msgget creates a message queue and returns identifier
	msgid = msgget(key, 0666 | IPC_CREAT);
	message.mesg_type = 1;
	
	//printf("Write Data : ");
        //fgets(message.mesg_text, 101, stdin);
        strcpy
        printf("name of the file: %s\n",name);
	//msgsnd to send message
	msgsnd(msgid, &message, sizeof(message), 0);

	//display the message
	printf("Data send is : %s \n", message.mesg_text);
	
	return 0;

}
