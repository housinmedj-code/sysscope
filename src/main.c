        #include <stdlib.h>
        #include <stdio.h>
        #include <string.h>
        #include <unistd.h>
        #include <sys/types.h>
        #include <sys/wait.h>

        void print_help(void){

            printf("option available : \n");
            printf("--cpu : display cpu information \n");
            printf("--memory : display memory information \n");
            printf("--disk: display disk information \n");
            printf("--file <filename> : display file information \n");

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

        int inspect_file(const char *path){

            pid_t pid = fork();

            if (pid==-1){
                perror("fork");
                return EXIT_FAILURE;
            }

            if (pid==0){
                /*child process*/
                execlp("file", "file", "--", path, (char *)NULL);

                perror("execlp");
                _exit(EXIT_FAILURE);

            }

            else if (pid>0){
                /*parent process*/

                int status;
                pid_t retour = waitpid(pid, &status, 0);

                if (retour == -1) {
                perror("waitpid");
                return EXIT_FAILURE;
                }
                
                if (WIFEXITED(status)){
                    int exit_status = WEXITSTATUS(status);
                    if (exit_status==0){
                        printf("file inspection completed successfully \n");
                        return EXIT_SUCCESS;
                    }
                    else{
                        printf("file inspection failed \n");
                        return EXIT_FAILURE;
                    }
                }
                printf("file process terminated abnormally\n");
                return EXIT_FAILURE;

            }

        


        }

        int main(int argc, char *argv[]) {

            printf("SysScope\n");

            if (argc==1){
                printf("please add an option \n");
                printf("to see the available options, please use --help \n");
                return EXIT_FAILURE;
            }

            if (argc>3){
                printf("too many arguments \n");
                printf("to see the available options, please use --help \n");
                return EXIT_FAILURE;
            }


            if(strcmp(argv[1], "--help")==0){
                print_help();
                return EXIT_SUCCESS;
            }

            if (strcmp(argv[1],"--cpu")==0){
                return run_command("lscpu");
            }

            if (strcmp(argv[1],"--memory")==0){
                return run_command("free -h");   
            }  

            if (strcmp(argv[1],"--disk")==0){
                return   run_command("df -h");   
            }

            if (strcmp(argv[1],"--file")==0){
                if (argc!=3){
                    printf("please provide a file name \n");
                    return EXIT_FAILURE;
                }
                printf("file to inspect : %s \n", argv[2]);
                return inspect_file(argv[2]);
            }
            
            printf("option not found \n");
            return EXIT_FAILURE;

        }

