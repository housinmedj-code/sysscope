#include <stdlib.h>
#include <stdio.h>


int main() {

    printf("SysScope -- CPU");

    char *commmand = "lscpu";

    int result = system(commmand);

    if (result==0){
        printf("command executed succesfully \n");
        return result;    
    }

    printf("command failed \n");
    return 1;

}

