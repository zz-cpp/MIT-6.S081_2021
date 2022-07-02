#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *file_name, char *target)
{

    int fd;
    struct stat st;
    struct dirent de;
    char buf[512], *p;

    if ((fd = open(file_name, 0)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", file_name);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", file_name);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        fprintf(2, "Usage: find dir file\n");
		exit(1);
        break;

    case T_DIR:

        if (strlen(file_name) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: path too long\n");
            break;
        }

        strcpy(buf, file_name);
        // p chotrol buf
        p = buf + strlen(buf);
        *p++ = '/';

        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
				continue;
            // assemply : /dir/ + (a || b || c) --> /dir/a /dir/b /dir/c
            memmove(p, de.name, DIRSIZ);
            // set string end
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf("ls: cannot stat %s\n", buf);
                continue;
            }

            if(st.type ==  T_DIR ){

                find(buf,target);
            }else if (st.type == T_FILE){
      				if (strcmp(de.name, target) == 0)
      				{
      					printf("%s\n", buf);
      				}
      			}
			}
			break;
        
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("find: need pramater /n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}