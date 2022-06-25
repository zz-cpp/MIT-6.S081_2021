#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *file_name, char *target)
{

    if(strcmp(".",file_name) == 0 || strcmp("..",file_name)== 0){
        return;
    }


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
            strcpy(buf,file_name);
            for(p=buf+strlen(buf); p >= buf && *p != '/'; p--);
            p++;
            if(strcmp(p,target) == 0){
                printf("%s \n", file_name);
                return;
            }
            return;
            break;
        

    case T_DIR:
    while(read(fd, &de, sizeof(de) == sizeof(de))){
        if(de.inum == 0)
        continue;

        strcpy(buf,file_name);
        p = buf + strlen(buf);
        *p++ = '/';
        memmove(p, de.name, sizeof(*de.name));
        find(p,target);
    }

    
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3){
    printf("find: need pramater /n");
    exit(1);
  }

  find(argv[1],argv[2]);
  exit(0);

}