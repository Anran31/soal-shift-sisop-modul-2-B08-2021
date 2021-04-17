#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <wait.h>
#include <dirent.h>
#include <time.h>

void generateKiller(const char *argv[],int sid)
{
    FILE *fptr;
    fptr = fopen("killer.sh","w");
    fputs("#!/bin/bash\n\n",fptr);
    char program[100];
    if(!strcmp(argv[1],"-z"))
    {
        sprintf(program,"killall -9 %s\nrm killer.sh",argv[0]);
        fputs(program,fptr);
    }
    else if (!strcmp(argv[1],"-x"))
    {
        sprintf(program,"kill -9 %d\nrm killer.sh",sid);
        fputs(program,fptr);
    }

    fclose(fptr);
}

void forkExec(char *command, char *argv[])
{
    pid_t child_id;
    int status;

    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        // this is child
        execv(command, argv);
    } else {
        // this is parent
        while ((wait(&status)) > 0);
        return;
    }    
}

void addSuccess(char *timeStamp)
{
    char msg[20] = "Download Success",ch;
    int i,key = 5;

    for(i = 0; msg[i] != '\0'; ++i){
		ch = msg[i];
		
		if(ch >= 'a' && ch <= 'z'){
			ch = ch + key;
			
			if(ch > 'z'){
				ch = ch - 'z' + 'a' - 1;
			}
			
			msg[i] = ch;
		}
		else if(ch >= 'A' && ch <= 'Z'){
			ch = ch + key;
			
			if(ch > 'Z'){
				ch = ch - 'Z' + 'A' - 1;
			}
			
			msg[i] = ch;
		}
	}

    char path[100];
    strcpy(path,timeStamp);
    strcat(path,"/");
    strcat(path,"status.txt");
    
    FILE *fptr;

    fptr = fopen(path, "w");
    fputs(msg, fptr);
    fclose(fptr);

    return;
}


void downloadPics(char *timeStamp)
{

    pid_t child_id;

    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        // this is child
            pid_t child_id_1;
            int status_1;

            child_id_1 = fork();

            if (child_id_1 < 0) {
                exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
            }

            if (child_id_1 == 0) {
                // this is child
                for(int i = 0; i<10; i++)
                {
                    time_t t = time(NULL);
                    struct tm curTime = *localtime(&t);
                    char fileTimeStamp[100];
                    char loc[100];
                    strftime(fileTimeStamp,sizeof(fileTimeStamp),"%Y-%m-%d_%H:%M:%S",&curTime);
                    clock_t start = time(NULL), end;
                    strcpy(loc,timeStamp);
                    strcat(loc,"/");
                    strcat(loc,fileTimeStamp);

                    char source[50] = "https://picsum.photos/";
                    char size[20];
                    sprintf(size,"%ld",t%1000+50);
                    strcat(source,size);
                    printf("%s\n",source);
                    char *arg0[] = {"wget","-bq",source, "-O", loc, NULL};
                    forkExec("/bin/wget",arg0);
                    end = time(NULL);
                    long int diff = end-start;
                    //if(i!=9)
                    //{
                        while(diff != 5)
                        {
                            sleep(1);
                            end = time(NULL);
                            diff = end-start;
                        }
                    //}
                    //else _exit(1);
                }

            } else {
                // this is parent
                while ((wait(&status_1)) > 0);
                addSuccess(timeStamp);
                //zip(timeStamp);
                char *arg[] = {"zip",timeStamp,"-qrm",timeStamp,NULL};
                forkExec("/bin/zip",arg);
                _exit(1);
            }
    } else {
        // this is parent
        return;
    }
}

void makeDir(char *timeStamp)
{
    pid_t child_id;
    int status;

    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        // this is child
            pid_t child_id_1;
            int status_1;

            child_id_1 = fork();

            if (child_id_1 < 0) {
                exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
            }

            if (child_id_1 == 0) {
                // this is child
                    pid_t child_id_2;
                    int status_2;

                    child_id_2 = fork();

                    if (child_id_2 < 0) {
                        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
                    }

                    if (child_id_2 == 0) {
                        // this is child
                        char *arg0[] = {"mkdir", timeStamp, NULL};
                        forkExec("/bin/mkdir",arg0);
                        _exit(1);
                    } else {
                        // this is parent
                        while ((wait(&status_2)) > 0);
                        downloadPics(timeStamp);
                        _exit(1);
                    }
            } else {
                // this is parent
                //while ((wait(&status_1)) > 0);

                _exit(1);
            }    

    } else {
        // this is parent
        while ((wait(&status)) > 0);
        return;
    }      
}

int main(int argc, const char *argv[])
{

        pid_t pid, sid;        // Variabel untuk menyimpan PID

        pid = fork();     // Menyimpan PID dari Child Process
        /* Keluar saat fork gagal
        * (nilai variabel pid < 0) */
        if (pid < 0) {
            exit(EXIT_FAILURE);
        }

        /* Keluar saat fork berhasil
        * (nilai variabel pid adalah PID dari child process) */
        if (pid > 0) {
            exit(EXIT_SUCCESS);
        }

        umask(0);

        sid = setsid();
        if (sid < 0) {
            exit(EXIT_FAILURE);
        }

        if ((chdir("/home/anran/kuliah/sisop/modul2/shift2/soal3")) < 0) {
            exit(EXIT_FAILURE);
        }

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

    if(!strcmp(argv[1],"-z") || !strcmp(argv[1],"-x"))
    {
        int int_sid = (int) sid;
        generateKiller(argv, int_sid);
        while (1) {
            // Tulis program kalian di sini
            time_t t = time(NULL);
            struct tm curTime = *localtime(&t);
            char timeStamp[100];
            clock_t start = time(NULL), end;
            //printf("%04d-%02d-%02d_%02d:%02d:%02d\n",curTime.tm_year+1900,curTime.tm_mon+1,curTime.tm_mday,curTime.tm_hour,curTime.tm_min, curTime.tm_sec);
            strftime(timeStamp,sizeof(timeStamp),"%Y-%m-%d_%H:%M:%S",&curTime);
            printf("%s\n",timeStamp);
            makeDir(timeStamp);
            end = time(NULL);
            long int diff = end-start;
            while(diff != 40)
            {
                sleep(1);
                end = time(NULL);
                diff = end-start;
            }
        }
    }
    else _exit(1);
}
