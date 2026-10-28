    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

    void print_help(void){
        printf("option available : \n");
        printf("--cpu : display cpu information \n");
        printf("--memory : display memory information \n");
        printf("--disk: display disk information \n");

    }

    int run_command(char *command){
        int result = system(command);

        if (result==0){
            printf("command executed succesfully \n");
            return EXIT_SUCCESS;    
        }
        printf("command failed \n");
        return EXIT_FAILURE;
    }

    int main(int argc, char *argv[]) {

        printf("SysScope\n");

        if (argc==1){
            printf("please add an option \n");
            printf("to see the available options, please use --help \n");
            return EXIT_FAILURE;
        }

        if (argc>2){
            printf("too many arguments \n");
            printf("to see the available options, please use --help \n");
            return EXIT_FAILURE;
        }


        if(strcmp(argv[1], "--help")==0){
            print_help();
            return EXIT_SUCCESS;
        }

        if (strcmp(argv[1],"--cpu")==0){
            run_command("lscpu");
        }

        if (strcmp(argv[1],"--memory")==0){
            run_command("free -h");   
        }  

        if (strcmp(argv[1],"--disk")==0){
            run_command("df -h");   
        }
        
        printf("option not found \n");
        return EXIT_FAILURE;
    }

