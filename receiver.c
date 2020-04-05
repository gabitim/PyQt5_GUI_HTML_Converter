// C Program for Message Queue (Reader Process)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <regex.h>

#define BUFF_SIZE 2048
#define MAX_SIZE 65362

// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[BUFF_SIZE];
} message;



int main()
{
   
    key_t key; //key
    int msgid; 
    FILE *file = NULL; //the file we write in
    regex_t regex; //the regex used to validate
 
    size_t PacketsReceived = 0;
    int nrOfTotalPackets = 0;
    int nrOfBytesReceived = 0;
    char full_html_message[MAX_SIZE]; 

    // ftok to generate unique key
    key = ftok("message_queue_name", 'B');

    // msgget creates a message queue
    // and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);

    // msgrcv to receive first message which is the name of the file, 
    msgrcv(msgid, &message, sizeof(message), 1, 0);
	   
    //we add the html extension
    char file_name[100], *ext = ".html";
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
        // display the message
        msgrcv(msgid, &message, sizeof(message), 1, 0);
        printf("the message: \n %s",message.mesg_text);        

        if ((regcomp(&regex, "<html>.*</html>", 0) &&
            regcomp(&regex, "<title>.*</title>", 0) &&
            regcomp(&regex, "<body>.*</body>", 0) &&
            regcomp(&regex, "<p>.*</p>", 0)
             )== 0)
        {
            if ((regexec(&regex, message.mesg_text, 0, NULL, 0) &&
            regexec(&regex, message.mesg_text, 0, NULL, 0) &&
            regexec(&regex, message.mesg_text, 0, NULL, 0) &&
            regexec(&regex, message.mesg_text, 0, NULL, 0)
            ) == 0)
            {
                printf("Message validation: correct \n");
                fprintf(file, "%s", message.mesg_text);
                printf("html_converter.html was created \n");
            }
            else
            {
                printf("Message validation: incorrect \n");
                fprintf(file, "Error");
            }
        }
        else
        {
            printf("Error \n");
            fprintf(file, "Error");  
        }
    }
    // to destroy the message queue
    msgctl(msgid, IPC_RMID, NULL);
    fclose(file);

    return 0;
}
