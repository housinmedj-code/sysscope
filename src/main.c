        #define _POSIX_C_SOURCE 200809L
        
        #include <stdlib.h>
        #include <stdio.h>
        #include <string.h>
        #include <unistd.h>
        #include <sys/types.h>
        #include <sys/wait.h>
        #include <sys/stat.h>
        #include <inttypes.h>
        #include <time.h>

        void print_help(void){

            printf("option available : \n");
            printf("--cpu : display cpu information \n");
            printf("--memory : display memory information \n");
            printf("--disk: display disk information \n");
            printf("--file <filepath> : display file information \n");

        }

        int print_file_metadata(const char *path){

            struct stat infos;
            int resultat = lstat(path, &infos);

            

            if (resultat==-1){
                perror("lstat");
                return EXIT_FAILURE;
            }

            

            printf("Size : %jd bytes\n", (intmax_t)infos.st_size);

            if (S_ISREG(infos.st_mode)){
                printf("Type : regular file\n");
            }
            else if (S_ISDIR(infos.st_mode)){
                printf("Type : directory\n");
            }
            else if (S_ISLNK(infos.st_mode)){
                printf("Type : symbolic link\n");
            }
            else{
                printf("Type : other\n");

            }

            /*last modification time*/
            char *date = ctime(&infos.st_mtime);
            if (date==NULL){
                fprintf(stderr, "ctime failed\n");
                return EXIT_FAILURE;
            }
            printf("Last modification time : %s", date);

            /*GID*/
            printf("Owner UID: %ju\n", (uintmax_t)infos.st_uid);
            printf("Owner GID: %ju\n", (uintmax_t)infos.st_gid);
            /* Permissions */
            printf("Permissions : ");
                /*propriétaire*/
            if(infos.st_mode & S_IRUSR){
                printf("r");
            }else{
                printf("-");
            }
            if (infos.st_mode & S_IWUSR){
                printf("w");
            }else{
                printf("-");
            }
            if (infos.st_mode & S_IXUSR){
                printf("x");
            }else{
                printf("-");
            }

                /*groupe*/
            if (infos.st_mode & S_IRGRP){
                printf("r");
            }else{
                printf("-");
            }

            if (infos.st_mode & S_IWGRP){
                printf("w");
            }else{
                printf("-");
            }

            if (infos.st_mode & S_IXGRP){
                printf("x");
            }else{
                printf("-");
            }

            /*autres*/

            if (infos.st_mode & S_IROTH){
                printf("r");
            }else{
                printf("-");
            }

            if (infos.st_mode & S_IWOTH){
                printf("w");
            }else{
                printf("-");
            }

            if (infos.st_mode & S_IXOTH){
                printf("x");
            }else{
                printf("-");
            }

            printf("\n");


            return EXIT_SUCCESS;
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

            else {
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
                return print_file_metadata(argv[2]);
                
            }
            
            printf("option not found \n");
            return EXIT_FAILURE;

        }

