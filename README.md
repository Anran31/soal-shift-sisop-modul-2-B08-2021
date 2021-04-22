# soal-shift-sisop-modul-2-B08-2021

## Daftar Isi

* [No 1](#no-1)
	* [1.1 (a)](#11)
	* [1.2 (b & c)](#12) 
	* [1.3 (d)](#13)
	* [1.4 (e)](#14)
	* [1.5 (f)](#15)
	
* [No 2](#no-2)
	* [2.1 (a)](#21)
	* [2.2 (b)](#22)
	* [2.3 (c)](#2.3)
	* [2.4 (d)](#2.4)
	* [2.5 (e)](#2.5)

* [No 3](#no-3)
	* [3.1 (a)](#3.1)
	* [3.2 (b)](#3.2)
	* [3.3 (c)](#3.3)
	* [3.4 (d)](#3.4)
	* [3.5 (e)](#3.5)



## No 1
Pada suatu masa, hiduplah seorang Steven yang hidupnya pas-pasan. Steven punya pacar, namun sudah putus sebelum pacaran. Ketika dia galau memikirkan mantan, ia selalu menonton https://www.youtube.com/watch?v=568DH_9CMKI untuk menghilangkan kesedihannya.
Pada hari ulang tahun Stevany, Steven ingin memberikan Stevany zip berisikan hal-hal yang disukai Stevany. Steven ingin isi zipnya menjadi rapi dengan membuat folder masing-masing sesuai extensi. 

Pertama tama adalah mendeklarasikan nama nama folder yang akan digunakan beserta id download dan link yang dibutuhkan

```c
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
```

### 1.1 
a. Dikarenakan Stevany sangat menyukai huruf Y, Steven ingin nama folder-foldernya adalah Musyik untuk mp3, Fylm untuk mp4, dan Pyoto untuk jpg 

Pertama, kita membuat program utama seperti dibawah ini :
```c
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
```
Dengan memakai ``time`` serta ``struct`` dan agar dapat mengambil bagian bagian yang dibutuhkan.
Sedang untuk membuat folder itu sendiri menggunakan fungsi dari ``mkdir``
```c
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
```

### 1.2
b & c. Untuk musik Steven mendownloadnya dari link di bawah, film dari link di bawah lagi, dan foto dari link dibawah juga :). Steven tidak ingin isi folder yang dibuatnya berisikan zip, sehingga perlu meng-extract-nya setelah didownload serta 

Untuk menyelesaikan permasalahan poin (b) serta (c), dibutuhkan fungsi ``unduh_extract`` serta ``util_unduh_and_extract``
```c
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
```
Disini menggunakan ``for`` sebanyak 3 karena file yang perlu di download ada di 3 folder terpisah. sedangkan penggunaan ``child_id`` adalah agar program utamanya tidak di-Terminate ketika ke fungsi ``util_unduh_and_extract`` yang menggunakan ``execv`` tidak serta merta mati dan tidak berjalan kembali.
```c
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
```
Menggunakan ``unizp`` untuk mem-unzip file serta ``"wget", "-q", "--no-check-certificate"`` untuk mengunduh file yang diinginkan. serta menggunakan ``wait`` agar dapat mengecek apakah file sudah di download apa belum , jika belum maka dia tidak akan mengaktifkan bagian unzip dalam fungsi.


### 1.3 
d. Memindahkannya ke dalam folder yang telah dibuat (hanya file yang dimasukkan).

Untuk soal poin (d) akan menggunakan fungsi ``move_files`` serta ``util_move_files``
```c
void move_files() {
    int i;
    for (i=0;i<3;i++)
        util_move_files(i);
}
```
Prinsip dari fungsi ``move_files`` sama dengan fungsi ``unduh_extract`` sebelumnya, yaitu menggunakan ``for`` untuk ketiga folder yang di butuhkan. hanya saja dia tidak menggunakan ``child_id`` dikarenakan sudah diaplikasikan di dalam fungsi ``util_move_files``

```c
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
```
Pada fungsi ``util_move_files`` menggunakan sistem ``Directory Listing``. Pertama tama yang dilakukan adalah mengambil directory satu persatu yang dibutuhkan, menggunakan
```c
dp = opendir(downloaded_dir_name[i]);
...
```
Lalu kemudian membaca isi directorynya sampai habis menggunakan
```c
while ((files = readdir(dp))) 
{
    ...
}
```
Kemudian memindahkannya satu persatu dengan ``execv``
```c
if (child_id == 0) {
                    char *argv[] = {"mv", filePath, dir_name[i], NULL};
                    execv ("/bin/mv", argv);
                }
```

### 1.4 
e. Untuk memudahkan Steven, ia ingin semua hal di atas berjalan otomatis 6 jam sebelum waktu ulang tahun Stevany).

Untuk soal poin (e) ini, maka akan menggunakan fungsi bernama ``programjam16``
```c
void programjam16 () {
    make_dir();
    unduh_extract();
    move_files();
}
```

### 1.5 
f. Setelah itu pada waktu ulang tahunnya Stevany, semua folder akan di zip dengan nama Lopyu_Stevany.zip dan semua folder akan di delete(sehingga hanya menyisakan .zip).

Untuk soal poin terakhir ini, akan menggunakan fungsi bernama ``programjam22``, ``archive``, dan ``removee``

Dalam fungsi ``archive`` akan melakukan ``zip`` pada folder folder yang dibutuhkan.
```c
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
```
Sedangkan pada fungsi ``removee``sendiri berfungsi untuk menghapus directory directory yang ada dan hanya akan menyisakan folder denagn format .zip saja.
```c
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
```
Pada fungsi ini, menggunakan ``rm`` sebagai command penghapus directory.

Dan kedua fungsi diatas ini, dimasukkan kedalam fungsi ``programjam22`` agar kedua fungsi itu berjalan pada waktu yang ditentukan.
```c
void programjam22() {
    archive();
    removee();
}
```



## No 2
Loba bekerja di sebuah petshop terkenal, suatu saat dia mendapatkan zip yang berisi banyak sekali foto peliharaan dan Ia diperintahkan untuk mengkategorikan foto-foto peliharaan tersebut. Loba merasa kesusahan melakukan pekerjaanya secara manual, apalagi ada kemungkinan ia akan diperintahkan untuk melakukan hal yang sama. Kamu adalah teman baik Loba dan Ia meminta bantuanmu untuk membantu pekerjaannya.

### 2.1 
a. Pertama-tama program perlu mengextract zip yang diberikan ke dalam folder “/home/[user]/modul2/petshop”. Karena bos Loba teledor, dalam zip tersebut bisa berisi folder-folder yang tidak penting, maka program harus bisa membedakan file dan folder sehingga dapat memproses file yang seharusnya dikerjakan dan menghapus folder-folder yang tidak dibutuhkan.

Pertama, kita membuat program utama seperti dibawah ini:

```c
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
```

Untuk menyelesaikan salah satu permasalahan di poin a, kita menggunakan suatu fungsi bernama ``unzipping`` yang berguna untuk membuat direktori seperti yang disuruh pada soal kemudian mengunzip zip yang sudah didownload ke direktori tersebut.

```c
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
```
Untuk permasalahan selanjutnya, yaitu menghapus folder yang tidak diperlukan, kita menggunakan salah satu bagian dari fungsi ``solution`` di bawah ini:

```c
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
```

Pertama kita melakukan directory listing, kemudian untuk setiap file di dalam direktori petshop, akan dicek apakah file tersebut berupa sebuah folder atau bukan menggunakan salah satu fungsi di library stat yaitu ``S_ISDIR(file.st_mode)``. Jika file yang dicek merupakan sebuah direktori, maka direktori tersebut akan dihapus, tetapi jika bukan, file foto akan dilanjutkan untuk diproses sesuai dengan perintah di poin-poin setelah ini. 

### 2.2 
b. Foto peliharaan perlu dikategorikan sesuai jenis peliharaan, maka kamu harus membuat folder untuk setiap jenis peliharaan yang ada dalam zip. Karena kamu tidak mungkin memeriksa satu-persatu, maka program harus membuatkan folder-folder yang dibutuhkan sesuai dengan isi zip.
Contoh: Jenis peliharaan kucing akan disimpan dalam “/petshop/cat”, jenis peliharaan kura-kura akan disimpan dalam “/petshop/turtle”.

Untuk poin b sampai selanjutnya, kita menggunakan fungsi ``makeFolderAndMovePhoto`` di bawah ini:

```c
    void makeFolderAndMovePhoto(char *name, char *source)
    {
        const char *p="_",*q=";";
        char *a,*b,*c,*d;
        char realName[100];
        strcpy(realName,name);

        removeSubstr(name, ".jpg");
        for( a=strtok_r(name,p,&c) ; a!=NULL ; a=strtok_r(NULL,p,&c) ) {
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
    }
```

Fungsi di atas pertama-tama menerima argumen berupa nama file yang akan di proses serta path ke folder petshop. Kemudian kita menghapus ".jpg" yang terdapat pada nama file menggunakan fungsi ``removeSubstr`` di bawah ini:

```c
    void removeSubstr (char *string, char *sub) {
        char *match;
        int len = strlen(sub);
        while ((match = strstr(string, sub))) {
            *match = '\0';
            strcat(string, match+len);
        }
    }
```

Karena pada setiap foto mungkin saja terdapat lebih dari 1 binatang, maka pertama kita harus membagi string nama file berdasarkan karakter "_" yang digunakan untuk memisahkan setiap hewan mengunakan looping ini:

```c
    for( a=strtok_r(name,p,&c) ; a!=NULL ; a=strtok_r(NULL,p,&c) )
    {
        ...
        ...
    }
```

Kemudian untuk setiap data satu peliharaan yang ada, kita membagi string tersebut berdasarkan karakter ";" dan menyimpannya dalam string ``type``, ``name``, serta ``age`` seperti di bawah ini:

```c
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
```

Setelah mendapatkan jenis hewan di foto tersebut yang disimpan dalam ``type``, maka kita dapat membuat folder berdasarkan jenis hewan tersebut menggunakan fungsi ``makeFolder_util`` di bawah ini:

```c
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
```

Karena pada poin e kita disuruh membuat sebuah file ``keterangan.txt`` yang berisi nama dan umur semua peliharaan dalam folder ini, maka setelah membuat folder ini, kita membuat juga file tersebut menggunakan fungsi ``makeKeterangan`` di bawah ini:

```c
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
```

### 2.3 
c. Setelah folder kategori berhasil dibuat, programmu akan memindahkan foto ke folder dengan kategori yang sesuai dan di rename dengan nama peliharaan.
Contoh: “/petshop/cat/joni.jpg”. 

### 2.4 
d. Karena dalam satu foto bisa terdapat lebih dari satu peliharaan maka foto harus di pindah ke masing-masing kategori yang sesuai. Contoh: foto dengan nama “dog;baro;1_cat;joni;2.jpg” dipindah ke folder “/petshop/cat/joni.jpg” dan “/petshop/dog/baro.jpg”.

### 2.5 
e. Di setiap folder buatlah sebuah file "keterangan.txt" yang berisi nama dan umur semua peliharaan dalam folder tersebut.

Untuk poin c, d, dan e, setelah membuat folder pada poin b, maka foto tersebut akan dicopy ke folder yang sesuai dengan ``type`` dan direname dengan ``name`` menggunakan fungsi ``copyPhoto`` di bawah ini:

```c
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
```

Setelah dicopy ke folder yang sesuai, kemudian kita menambahkan detail hewan yang baru saja ditambahkan ke ``keterangan.txt`` pada folder tersebut menggunakan fungsi ``addKeterangan`` :
```c
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
```

Setelah sebuah foto selesai diproses, maka kita perlu menghapus foto asli yang masih berada di direktori petshop menggunakan fungsi ``deletePhoto`` :

```c
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
```



## No 3
Ranora adalah mahasiswa Teknik Informatika yang saat ini sedang menjalani magang di perusahan ternama yang bernama “FakeKos Corp.”, perusahaan yang bergerak dibidang keamanan data. Karena Ranora masih magang, maka beban tugasnya tidak sebesar beban tugas pekerja tetap perusahaan. Di hari pertama Ranora bekerja, pembimbing magang Ranora memberi tugas pertamanya untuk membuat sebuah program.

### 3.1 
a. Ranora harus membuat sebuah program C yang dimana setiap 40 detik membuat sebuah direktori dengan nama sesuai timestamp [YYYY-mm-dd_HH:ii:ss].

Pertama tama adalah menulis ``main`` dari program 
```c
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
            //printf("%s\n",timeStamp);
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
```
Pada ``main``, digunakan prompt ``chdir`` karena itu untuk memindah directory dikarenakan script yang diganti. 

Lalu untuk soal (a) sendiri, menggunakan ``time`` sebagai timer untuk mengecek apakah sudah terjalan selama 40 detik. Jika iya, maka akan dijalankan 
```c
strftime(timeStamp,sizeof(timeStamp),"%Y-%m-%d_%H:%M:%S",&curTime);
```
Dengan ``timeStamp`` sebagai destination, ``curTime`` sebagai pointer untuk memberikan directory dengan nama sesuai format dengan menggunakan library ``tm`` dengan structure ``localtime``. Lalu untuk ``sleep`` digunakan jika ``time`` belum mencapai 40 detik, maka akan sleep terlebih dahulu, jika sudah terpenuhi maka dia akan melanjutkan kembali, berulang ulang.

Setelah itu, hasil dari ``strftime`` akan masuk ke dalam fungsi ``makeDir``
```c
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
```
``fork`` ditulis sebanyak 3 kali karena setiap 40 detik program akan berjalan, yang pertama berjalan adalah ``mkdir`` lalu kemudian menuju ke ``wait`` untuk menuju ke fungsi ``downloadPics``


### 3.2 
b. Setiap direktori yang sudah dibuat diisi dengan 10 gambar yang didownload dari https://picsum.photos/, dimana setiap gambar akan didownload setiap 5 detik. Setiap gambar yang didownload akan diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss] dan gambar tersebut berbentuk persegi dengan ukuran (n%1000) + 50 pixel dimana n adalah detik Epoch Unix.

Pada soal poin (b), menggunakan fungsi ``downloadPics``. di dalamnya ada fungsi ``for`` yang digunakan untuk loop sebanyak 10 kali 
```c

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
```             
Didalam ``for``, memakai ``strftime`` untuk ke tempat yang sama. Lalu untuk sizenya, menggunakan ``t%1000+50`` karena ``t`` itu sendiri sudah berbentuk Epoch Unix, dan tinggal di masukkan atau ditempel pada source yang sudah dituliskan. Setelah itu, mendownload file menggunakan ``char *arg0[] = {"wget","-bq",source, "-O", loc, NULL};``. Untuk ``time`` sendiri adalah timer yang digunakan jika ``diff`` telah mencapai 5, maka akan mengulang download untuk file selanjutnya, jika tidak, maka akan menjalankan ``sleep``. 


### 3.3
c. Setelah direktori telah terisi dengan 10 gambar, program tersebut akan membuat sebuah file “status.txt”, dimana didalamnya berisi pesan “Download Success” yang terenkripsi dengan teknik Caesar Cipher dan dengan shift 5. Caesar Cipher adalah Teknik enkripsi sederhana yang dimana dapat melakukan enkripsi string sesuai dengan shift/key yang kita tentukan. Misal huruf “A” akan dienkripsi dengan shift 4 maka akan menjadi “E”. Karena Ranora orangnya perfeksionis dan rapi, dia ingin setelah file tersebut dibuat, direktori akan di zip dan direktori akan didelete, sehingga menyisakan hanya file zip saja.

Di poin ini, menggunakan sebagian fungsi ``downloadPics`` dan ``addSuccess``.
```c
            } else {
                // this is parent
                while ((wait(&status_1)) > 0);
                addSuccess(timeStamp);
                //zip(timeStamp);
                char *arg[] = {"zip",timeStamp,"-qrm",timeStamp,NULL};
                forkExec("/bin/zip",arg);
                _exit(1);
            }

```
Setelah sukses mendownload 10 gambar, maka dia akan menuju fungsi ``addSuccess``

```c
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
```
Dalam fungsi ``addSuccess`` akan membuat file ``.txt`` untuk menyimpan keterangan "Download Success". Dengan ``key=5`` untuk enskripsi Caesar Cipher.
```c
//eskripsi Caesar Cipher
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
```
Setelah enkripsi dijalankan, directory "status.txt" akan dibuat. Dan dibuka dengan command ``w`` lalu menaruh Cesar Cipher menggunakan ``fputs(msg, fptr);`` dan ditutup kembali ``fclose(fptr);``.

Setelah sukses menambahkan file "status.txt", maka file itu akan di zip menggunakan
```c
char *arg[] = {"zip",timeStamp,"-qrm",timeStamp,NULL};
                forkExec("/bin/zip",arg);
```
Pada fungsi ``downloadPics``.


### 3.4 
d. Untuk mempermudah pengendalian program, pembimbing magang Ranora ingin program tersebut akan men-generate sebuah program “Killer” yang executable, dimana program tersebut akan menterminasi semua proses program yang sedang berjalan dan akan menghapus dirinya sendiri setelah program dijalankan. Karena Ranora menyukai sesuatu hal yang baru, maka Ranora memiliki ide untuk program “Killer” yang dibuat nantinya harus merupakan program bash.

Poin (d) menggunakan sebagian fungsi di ``main`` sebagai execute nya dan fungsi ``generateKiller`` sendiri. Untuk fungsi di``main`` dibutuhkan untuk mengetahui id dari parent prosesnya agar dapat di-kill serta argumen valuenya.
```c
if(!strcmp(argv[1],"-z") || !strcmp(argv[1],"-x"))
    {
        int int_sid = (int) sid;
        generateKiller(argv, int_sid);
        ...
        ...
    }
```
Sedang fungsi ``generateKiller``sendiri berjalan dengan menggunakan informasi id parent dan argv yang didapatkan dari fungsi ``main``.
```c
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
```
Pertama adalah membuat file "killer.sh" karena yang diminta adalah berupa **bash**. jika argv dari ``main`` adalah ``-z`` maka akan menjalankan 
```c
sprintf(program,"killall -9 %s\nrm killer.sh",argv[0]);
        fputs(program,fptr);
 ```
 Dengan ``%s`` adalah nama program yang didapatkan dari ``argv(0)`` lalu ``/nrm killer.sh`` digunakan untuk meremove file **killer.sh** ketika selesai di jalankan. ``fputs`` digunakan untuk menaruh rangkaian perintah ini di dalam ``fptr``
 
 Sedangkan jika argv yang didapatkan adalah ``-x`` maka akan dijalankan 
 ```c
 sprintf(program,"kill -9 %d\nrm killer.sh",sid);
        fputs(program,fptr);
 ```
Dengan ``%d`` berupa id parent prosesnya didapat dari ``sid`` lalu sama seperti sebelumnya, akan diremove ketika selesai dijalankan.


### 3.5
e. Pembimbing magang Ranora juga ingin nantinya program utama yang dibuat Ranora dapat dijalankan di dalam dua mode. Untuk mengaktifkan mode pertama, program harus dijalankan dengan argumen -z, dan Ketika dijalankan dalam mode pertama, program utama akan langsung menghentikan semua operasinya Ketika program Killer dijalankan. Sedangkan untuk mengaktifkan mode kedua, program harus dijalankan dengan argumen -x, dan Ketika dijalankan dalam mode kedua, program utama akan berhenti namun membiarkan proses di setiap direktori yang masih berjalan hingga selesai (Direktori yang sudah dibuat akan mendownload gambar sampai selesai dan membuat file txt, lalu zip dan delete direktori).

Poin ini ada dalam fungsi ``main``
```c
if(!strcmp(argv[1],"-z") || !strcmp(argv[1],"-x"))
    {
        ...
        ...
    }
```

Digunakan dan ditaruh sebelum command yang lain, jadi semua proses pada fungsi lain akan melewati argumen ini terlebih dahulu.
