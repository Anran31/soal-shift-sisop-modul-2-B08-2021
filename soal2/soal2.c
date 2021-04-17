#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>


void unzipping(char dest[]);
void solution(char *dest);
void removeFolder(char *source);
void makeFolderAndMovePhoto(char *name, char *source);
void removeSubstr (char *string, char *sub);
void makeFolder_util(char *source, char *name);
void copyPhoto(char *source,char *realName,char *type,char *name,char *age);
void makeKeterangan(char *path);
void addKeterangan(char *keterangan,char *name,char *age);
void deletePhoto(char *source,char *realName);


int main() {
  pid_t child_id;
  int status;
  char workSpace[] = "/home/anran/modul2/petshop";
  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
  }

  if (child_id == 0) {
    // this is child
    unzipping(workSpace);
  } else {
    // this is parent
    while ((wait(&status)) > 0);
    solution(workSpace);
  }
  return 0;
}

void unzipping(char dest[])
{
    pid_t child_id;
    int status;

    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        // this is child
        char *argv[] = {"mkdir", "-p", dest, NULL};
        execv("/bin/mkdir", argv);
    } else {
        // this is parent
        while ((wait(&status)) > 0);
        char *argv[] = {"unzip","-q", "/home/anran/kuliah/sisop/modul2/shift2/soal2/pets.zip","-d",dest, NULL};
        execv("/usr/bin/unzip", argv);
    }   
}

void solution(char *source)
{
    DIR *dp;
    struct dirent *ep;
    char path[100];
    strcpy(path,source);
    dp = opendir(source);

    if (dp != NULL)
    {
      while ((ep = readdir (dp))) {
          if (strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0)
          {
                char fullPath[100];
                strcpy(fullPath,path);
                strcat(fullPath,"/");
                strcat(fullPath,ep->d_name);
                //puts (fullPath);

                struct stat fs;
                int r;
                r = stat(fullPath,&fs);
                if( r==-1 )
                {
                    fprintf(stderr,"File error\n");
                    exit(1);
                }
                if( S_ISDIR(fs.st_mode) )
                    //printf("%s is a directory\n",fullPath);
                    removeFolder(fullPath);
                else
                    makeFolderAndMovePhoto(ep->d_name,source);
          }
      }

      (void) closedir (dp);
    } else perror ("Couldn't open the directory"); 
}

void removeFolder(char *source)
{
    pid_t child_id;
    int status;

    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        // this is child

        char *argv[] = {"rm", "-r", source, NULL};
        execv("/bin/rm", argv);
    } else {
        // this is parent
        while ((wait(&status)) > 0);
        return;
    }   
}


void removeSubstr (char *string, char *sub) {
    char *match;
    int len = strlen(sub);
    while ((match = strstr(string, sub))) {
        *match = '\0';
        strcat(string, match+len);
    }
}

void makeFolderAndMovePhoto(char *name, char *source)
{
    const char *p="_",*q=";";
    char *a,*b,*c,*d;
    char realName[100];
    strcpy(realName,name);

    removeSubstr(name, ".jpg");
    //printf("%s\n",name);
    for( a=strtok_r(name,p,&c) ; a!=NULL ; a=strtok_r(NULL,p,&c) ) {
        //printf("%s\n",a);
        int index = 0;
        char type[20];
        char name[20];
        char age[20];
        for( b=strtok_r(a,q,&d) ; b!=NULL ; b=strtok_r(NULL,q,&d) )
        {
            if(index == 0) strcpy(type,b);
            else if(index == 1) strcpy(name,b);
            else if (index == 2) strcpy(age,b);
            index++;
        }
        makeFolder_util(source,type);
        copyPhoto(source,realName,type,name,age);
    }

    deletePhoto(source,realName);
    //printf("%s\n",realName);
}

void makeFolder_util(char *source,char *name)
{
    char path[100];
    strcpy(path,source);
    strcat(path,"/");
    strcat(path,name);

    pid_t child_id;
    int status;

    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        // this is child

        char *argv[] = {"mkdir", "-p", path, NULL};
        execv("/bin/mkdir", argv);
    } else {
        // this is parent
        while ((wait(&status)) > 0);
        makeKeterangan(path);
        return;
    }
}

void makeKeterangan(char *path)
{
    char dest[100];
    strcpy(dest,path);
    strcat(dest,"/");
    strcat(dest,"keterangan.txt");

    pid_t child_id;
    int status;

    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        // this is child

        char *argv[] = {"touch", dest, NULL};
        execv("/bin/touch", argv);
    } else {
        // this is parent
        while ((wait(&status)) > 0);
        return;
    }    
}

void copyPhoto(char *source,char *realName,char *type,char *name,char *age)
{
    char start[100];
    char dest[100];
    char keterangan[100];

    strcpy(start,source);
    strcat(start,"/");
    strcat(start,realName);

    strcpy(dest,source);
    strcat(dest,"/");
    strcat(dest,type);
    strcat(dest,"/");
    strcat(dest,name);
    strcat(dest,".jpg");

    strcpy(keterangan,source);
    strcat(keterangan,"/");
    strcat(keterangan,type);
    strcat(keterangan,"/");
    strcat(keterangan,"keterangan.txt");

    pid_t child_id;
    int status;

    child_id = fork();

    if (child_id < 0) {
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
    // this is child
        char *argv[] = {"cp", start,dest, NULL};
        execv("/bin/cp", argv);
    } else {
    // this is parent
    while ((wait(&status)) > 0);
        addKeterangan(keterangan,name,age);
        return;
    }
}

void addKeterangan(char *keterangan,char *name,char *age)
{
    char text[100];
    sprintf(text,"nama : %s\numur : %s\n\n",name,age);

    FILE *fPtr;
    fPtr = fopen(keterangan, "a");

    if (fPtr == NULL)
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open '%s' file.\n", keterangan);
        printf("Please check whether file exists and you have write privilege.\n");
        exit(EXIT_FAILURE);
    }

    fputs(text, fPtr);
    fclose(fPtr);

    return;

}

void deletePhoto(char *source,char *realName)
{
    char target[100];
    strcpy(target,source);
    strcat(target,"/");
    strcat(target,realName);

    pid_t child_id;
    int status;

    child_id = fork();

    if (child_id < 0) {
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
    // this is child
        char *argv[] = {"rm", target, NULL};
        execv("/bin/rm", argv);
    } else {
    // this is parent
    while ((wait(&status)) > 0);
        return;
    }

}
