#include "types.h"
#include "user.h"
#include "fcntl.h"


void testFtag(char *filePath){
    int fd;
    fd = open(filePath, O_RDWR);
    if(fd){
        // 0 - success, -1 - failed
        if(ftag(fd, "testKey", "testValue") == 0)
            printf(1, "ftag test success\n");
        else
            printf(1, "ftag test fail\n");
    }
    else
        printf(1,"testFtag: filePath couldn't open\n");

    close(fd);
}   

void testFuntag(char *filePath){
    int fd;
    fd = open(filePath, O_RDWR);
    if(fd){
        if(funtag(fd, "testKey") == 0)
            printf(1, "funtag test success\n");
        else
            printf(1, "funtag test fail\n");
    }
    else
        printf(1,"testFuntag: filePath couldn't open\n");

    close(fd);
}

void testGettag(char *filePath){
    int fd;
    char *buf = 0;
    int valueLen;

    fd = open(filePath, O_RDWR);
    valueLen = gettag(fd, "testKey", buf);
   
    if(fd){
        if(valueLen == strlen("testValue")){
            printf(1, "fgettag test success\n");
        }
        else
            printf(1, "fgettag test fail\n");
    }
    else
        printf(1,"testGettag: filePath couldn't open\n");

    close(fd);
}

int main(int argc, char *argv[])
{
    testFtag("./echo");
    testGettag("./echo");
    testFuntag("./echo");
    exit();
}
