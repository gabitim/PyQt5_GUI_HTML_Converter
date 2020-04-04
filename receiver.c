// C Program for Message Queue (Reader Process)
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <regex.h>
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[1000];
} message;

int main()
{
    // gcc receiver.c -o receiver
    // ./receiver
    key_t key;
    int msgid;
    FILE *file = NULL;
    regex_t regex;

    // ftok to generate unique key
    key = ftok("message_queue_name", 'B');

    // msgget creates a message queue
    // and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);

    // msgrcv to receive message
    msgrcv(msgid, &message, sizeof(message), 1, 0);

    // display the message

    file = fopen("html_converter.html","w");
    if(file == NULL)
        {
            printf("Error: file could not be open\n");
            msgctl(msgid, IPC_RMID, NULL);  
            exit(EXIT_FAILURE);
            return 1;
        }
    else
    {
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
