#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

void runcmd(char *com, char **argv)
{
    if (com[0] == 0)
    {
        exit(1);
    }

    // exec will autolly stop
    // printf("cmd: %s \n", com);

    int state = exec(com, argv);
     printf("state: %d \n", state);
     exit(1);
}

int readline(char **container)
{

    char buf[1024];
    char *p = buf;
    char *line;
    int count = 0;

    while (read(0, p, 1))
    {
        // printf("p: %c \n",*p);
        if ( *p == '\n' )
        {
            break;
        }
        p++;
        count++;
    }

    
    if(buf[0]==0 || count == 0 ){
        return 1;
    }

    *p = 0;
    int len = strlen(buf);
    
    // printf("len : %d \n",len);
    line = (char *)malloc(len + 1);
    strcpy(line, buf);
    // printf("line: %s ",line);
    // printf("line count: %d \n",count);
    // printf("line_content_read_line: %d \n",line);
    *container = line;
    return 0;
}

// start == 1
int spilt_blank(char *line, char **container, int curr_argc)
{

    // printf("spilt_blank \n");
    char *p = line;
    // printf("line_content: %s \n",line);
    int distance = 0;
    int len = strlen(line) + 1;

    for (int i = 0; i < len; i++)
    {
        distance++;

        if (p[i] == ' ' || i == (len -1))
        {
            p[i] = 0;
            container[curr_argc++] = &p[i - distance + 1];
            // printf("container: %s",container[curr_argc -1]);
            distance = 0;
          
        }
        
         
    }

    return curr_argc;
}

int main(int argc, char *argv[])
{

    // first run the command in xargs
    char *new_argv[MAXARG];
    for (int i = 1; i < argc; i++)
    {
        new_argv[i - 1] = argv[i];
    }

    // why must do copy not directlly give argv[1]
    char *command = malloc(strlen(argv[1]) + 1);
    strcpy(command, argv[1]);


    // second take in paramater from
    char* line = 0;
    while (readline(&line) == 0)
    {
        // printf("line_main: %d \n",line);
         spilt_blank(line, new_argv, argc - 1);
         // printf("command : %s \n",command);


        if(fork() == 0)
        {
            // printf("fork : \n ");
            runcmd(command, new_argv);
        
        }
        wait(0);

        for (int i = argc - 1; i < MAXARG; i++)
        {
           new_argv[i] = 0;
        }

    }

  
    exit(0);
}