// C Program for Message Queue (Reader Process)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <regex.h>

#define BUFF_SIZE 2000
#define MAX_SIZE 100000

// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[BUFF_SIZE];
} message;



int main()
{
	int i;    

    key_t key; //key
    int msgid; 
    FILE *file = NULL; //the file we write in
    regex_t regex; //the regex used to validate
 
    ///we recieve chunks from the file when its to large; and we put it back togheter;
  
    int nrOfTotalPackets = 0;
   
    char full_html_message[MAX_SIZE]; 

	//for file name
	char file_name[100];
	char *ext = ".html";

    // ftok to generate unique key
    key = ftok("message_queue_name", 'B');

    // msgget creates a message queue
    // and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);

    // msgrcv to receive first message which is the name of the file, 
    msgrcv(msgid, &message, sizeof(message), 1, 0);
    //printf("garbage: %s \n",message.mesg_text);

    msgrcv(msgid, &message, sizeof(message), 1, 0);
	printf("the file name: %s \n",message.mesg_text);
   
    //we add the html extension
    
    strcpy(file_name,message.mesg_text);
    strcat(file_name,ext);
	
	//we create and open the file and we check.    
	file = fopen(file_name,"wb");
    if(file == NULL)
        {
            printf("Error: file could not be open\n");
            msgctl(msgid, IPC_RMID, NULL);  
            exit(EXIT_FAILURE);
            return 1;
        }
    else
    {
        //number of chunks
        msgrcv(msgid, &message, sizeof(message), 1, 0);
        
	//convert from string to int   
	sscanf(message.mesg_text,"%d", &nrOfTotalPackets);
	printf("nr of chunks : %d \n",nrOfTotalPackets);
		
	for(i = 1; i <= nrOfTotalPackets; i++)
	{
		
		msgrcv(msgid, &message, sizeof(message), 1, 0); // we read the chunks
		
		strcat(full_html_message, message.mesg_text); //we put chunks back togheter
		memset(message.mesg_text,0, 2000); //we clear the buffer
		
	 	
	}	     
	
	//printf("full message\n %s", full_html_message);
	
	
        if ((regcomp(&regex, "<html>.*</html>", 0) &&
            regcomp(&regex, "<title>.*</title>", 0) &&
            regcomp(&regex, "<body>.*</body>", 0) &&
            regcomp(&regex, "<p>.*</p>", 0)
             )== 0)
        {
            if ((regexec(&regex, full_html_message, 0, NULL, 0) &&
            regexec(&regex, full_html_message, 0, NULL, 0) &&
            regexec(&regex, full_html_message, 0, NULL, 0) &&
            regexec(&regex, full_html_message, 0, NULL, 0)
            ) == 0)
            {
                printf("Message validation: correct \n");
                fprintf(file, "%s", full_html_message); //we write into the new html file
                printf("html_converter.html was created \n");
            }
            else
            {
                printf("TEMPORARY ERROR, WORKING IN PROGRESS \n Message validation: incorrect, file still created \n");
		fprintf(file, "%s", full_html_message); // this is temporary; because of the validation error; MUST be fixed;
                fprintf(file, "Error at validation; will be fixed\n");
            }
        }
        else
        {
            printf("Error at validation; will be fixed \n");
            fprintf(file, "Error at validation; will be fixed\n");  
        }
    }
    // to destroy the message queue
    msgctl(msgid, IPC_RMID, NULL);
    fclose(file);

    return 0;
}
