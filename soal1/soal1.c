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
#include <time.h>
#include <dirent.h>

char *dir_name[] = {"Musyik", "Fylm", "Pyoto"};
char *download_file_name[] = {"Musik_for_Stefany.zip", "Film_for_Stefany.zip", "Foto_for_Stefany.zip"};
char *download_id[] = {"1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J", "1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp", "1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD"};
char *downloaded_dir_name[] = {"MUSIK", "FILM", "FOTO"};
char depan[] = "https://drive.google.com/uc?id=";
char belakang[] = "&export=download";

void make_dir() {
    pid_t child_id;
    child_id = fork();

    if (child_id < 0)
        exit(EXIT_FAILURE);
    if (child_id == 0) {
        char *argv[] = {"mkdir", dir_name[0], dir_name[1], dir_name[2], NULL};
        execv("/bin/mkdir", argv);
    }
}

void util_unduh_and_extract(int i) {
    char string[100];
    sprintf (string, "%s%s%s", depan, download_id[i], belakang);
    pid_t child_id;
    child_id = fork();
    int status_download;

    if (child_id < 0)
        exit(EXIT_FAILURE);
    if (child_id == 0) {
        char *argv[] = {"wget", "-q", "--no-check-certificate", string, "-O", download_file_name[i], NULL};
        execv("/bin/wget", argv);
    }
    else {
        while (wait(&status_download) > 0);
        // sleep(10);
        char *argv[] = {"unizp", "-qq", download_file_name[i], NULL};
        execv("/bin/unzip", argv);
    }
}

void unduh_extract() {
    int i;
    for (i=0;i<3;i++) {
        pid_t child_id;
        child_id = fork();
        int status;

        if (child_id < 0)
            exit(EXIT_FAILURE);
        if (child_id == 0) 
            util_unduh_and_extract(i);
        while(wait(&status) > 0);
    }
}

void util_move_files(int i) {
    DIR *dp;
    struct dirent *files;
    // printf ("Mlebu kene\n");
    dp = opendir(downloaded_dir_name[i]);

    if (dp != NULL) {
        while ((files = readdir(dp))) {
            if (strcmp(files->d_name, ".") != 0 && strcmp(files->d_name, "..") != 0) {
                char filePath[500];
                sprintf (filePath, "%s%s%s", downloaded_dir_name[i], "/", files->d_name);
                // printf ("%s\n", filePath);
                pid_t child_id;
                child_id = fork();
                int status;
                if (child_id < 0)
                    exit (EXIT_FAILURE);
                if (child_id == 0) {
                    char *argv[] = {"mv", filePath, dir_name[i], NULL};
                    execv ("/bin/mv", argv);
                }
                while(wait(&status) > 0);
            }
        }
        (void) closedir (dp);
    }
    else perror ("Couldn't open the directory");
    return;
}

void move_files() {
    int i;
    for (i=0;i<3;i++)
        util_move_files(i);
}

void archive() {
    pid_t child_id;
    child_id = fork();
    int status;

    if (child_id < 0)
        exit(EXIT_FAILURE);
    if (child_id == 0) {
        char *argv[] = {"zip", "-qqr", "Lopyu_Stefany.zip", dir_name[0], dir_name[1], dir_name[2], NULL};
        execv("/bin/zip", argv);
    }
    while(wait(&status) > 0);
}
void removee() {
    pid_t child_id;
    child_id = fork();
    int status;

    if (child_id < 0)
        exit(EXIT_FAILURE);
    if (child_id == 0) {
        char *argv[] = {"rm", "-r", dir_name[0], dir_name[1], dir_name[2], 
                        downloaded_dir_name[0], downloaded_dir_name[1], downloaded_dir_name[2], NULL};
        execv("/bin/rm", argv);
    }
    while(wait(&status) > 0);
}
void programjam16 () {
    make_dir();
    unduh_extract();
    move_files();
}

void programjam22() {
    archive();
    removee();
}

int main () {
    pid_t pid, sid;
    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);
    
    umask(0);
    sid = setsid();

    if (sid < 0)
        exit(EXIT_FAILURE);
    if ((chdir("/home/fauzan/sisop/shift2/soal1")) < 0)
        exit(EXIT_FAILURE);
    
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) {
        time_t now = time(NULL);
        struct tm *full = localtime (&now);
        if (full->tm_mday == 9 && full->tm_mon == 3 && full->tm_hour == 16 && full->tm_min == 22 && full->tm_sec == 0)
            programjam16();
        else if (full->tm_mday == 9 && full->tm_mon == 3 && full->tm_hour == 22 && full->tm_min == 22 && full->tm_sec == 0)
            programjam22();
    }
    // programjam16();
    // programjam22();
}