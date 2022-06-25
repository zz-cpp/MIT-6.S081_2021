/*
p = get a number from left neighbor
print p
loop:
    n = get a number from left neighbor
    if (p does not divide n)
        send n to right neighbor
*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/*
    tip:
        the order of close(p[]) will affect your program process which will try out of your resourse of file descripters.

*/



// p is pipe and length is 2.
void primes_sieve(int * p)
{
    int start;
    int channel[2];

    close(p[1]);
    pipe(channel);

  
    if (read(p[0], &start, 4) != 4)
    {
        exit(0);
    }

    printf("prime %d \n", start);

    if (fork() == 0)
    {
        primes_sieve(channel);
    }
    else
    {
        int prime;
        close(channel[0]);

        while (1)
        {
            if (read(p[0], &prime, 4) != 4)
            {
                break;
            }

            if ((prime) % (start) != 0)
            {

                if (write(channel[1], &prime, 4) != 4)
                {
                    fprintf(2, "write failly");
                    exit(1);
                }
            }
        }
         close(channel[1]);

    }

        close(p[0]);
        wait(0);
        exit(0);

}

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    int start = 2;

    printf("prime %d \n", start);

  

    if (fork() == 0)
    {
        primes_sieve(p);
    }
    else
    {
          close(p[0]);

        for (int i = 3; i <= 35; i++)
        {

            if (i % start != 0)
            {
                if (write(p[1], &i, 4) != 4)
                {

                    fprintf(2, "write failly");
                    exit(1);
                }
            }
        }
    }

    close(p[1]);

    wait(0);
    exit(0);
}