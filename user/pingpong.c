#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
    int p[2];
    pipe(p);
    if (fork() == 0)
    {
        char buff[2];
        int child = getpid();
        if (read(p[0], buff, 1) != 1)
        {
            fprintf(2, "child read failly \n");
            exit(2);
        }

        printf("%d: received ping\n", child);
        close(p[0]);
       

        if (write(p[1], "c", 1) != 1)
        {
            fprintf(2, "child write failly \n");
            exit(2);
        }
        close(p[1]);
        exit(0);
    }

    char buff[2];
    int parent = getpid();

    if (write(p[1], "p", 1) != 1)
    {
        fprintf(2, "parent write failly \n");
        exit(2);
    }
    close(p[1]);
    wait(0);

    if (read(p[0], buff, 1) != 1)
    {
        fprintf(2, "parent read failly \n");
        exit(2);
    }

    printf("%d: received pong\n", parent);
    close(p[0]);
    exit(0);
}