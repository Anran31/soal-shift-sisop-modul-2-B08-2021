# soal-shift-sisop-modul-2-B08-2021

## No 1
Pada suatu masa, hiduplah seorang Steven yang hidupnya pas-pasan. Steven punya pacar, namun sudah putus sebelum pacaran. Ketika dia galau memikirkan mantan, ia selalu menonton https://www.youtube.com/watch?v=568DH_9CMKI untuk menghilangkan kesedihannya.
Pada hari ulang tahun Stevany, Steven ingin memberikan Stevany zip berisikan hal-hal yang disukai Stevany. Steven ingin isi zipnya menjadi rapi dengan membuat folder masing-masing sesuai extensi. 

(a) Dikarenakan Stevany sangat menyukai huruf Y, Steven ingin nama folder-foldernya adalah Musyik untuk mp3, Fylm untuk mp4, dan Pyoto untuk jpg 

(b) untuk musik Steven mendownloadnya dari link di bawah, film dari link di bawah lagi, dan foto dari link dibawah juga :). 

(c) Steven tidak ingin isi folder yang dibuatnya berisikan zip, sehingga perlu meng-extract-nya setelah didownload serta 

(d) memindahkannya ke dalam folder yang telah dibuat (hanya file yang dimasukkan).

(e) Untuk memudahkan Steven, ia ingin semua hal di atas berjalan otomatis 6 jam sebelum waktu ulang tahun Stevany). 

(f) Setelah itu pada waktu ulang tahunnya Stevany, semua folder akan di zip dengan nama Lopyu_Stevany.zip dan semua folder akan di delete(sehingga hanya menyisakan .zip).

Kemudian Steven meminta bantuanmu yang memang sudah jago sisop untuk membantunya mendapatkan hati Stevany.

## No 2
Loba bekerja di sebuah petshop terkenal, suatu saat dia mendapatkan zip yang berisi banyak sekali foto peliharaan dan Ia diperintahkan untuk mengkategorikan foto-foto peliharaan tersebut. Loba merasa kesusahan melakukan pekerjaanya secara manual, apalagi ada kemungkinan ia akan diperintahkan untuk melakukan hal yang sama. Kamu adalah teman baik Loba dan Ia meminta bantuanmu untuk membantu pekerjaannya.

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

c. Setelah folder kategori berhasil dibuat, programmu akan memindahkan foto ke folder dengan kategori yang sesuai dan di rename dengan nama peliharaan.
Contoh: “/petshop/cat/joni.jpg”. 

d. Karena dalam satu foto bisa terdapat lebih dari satu peliharaan maka foto harus di pindah ke masing-masing kategori yang sesuai. Contoh: foto dengan nama “dog;baro;1_cat;joni;2.jpg” dipindah ke folder “/petshop/cat/joni.jpg” dan “/petshop/dog/baro.jpg”.

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

a. Ranora harus membuat sebuah program C yang dimana setiap 40 detik membuat sebuah direktori dengan nama sesuai timestamp [YYYY-mm-dd_HH:ii:ss].

b. Setiap direktori yang sudah dibuat diisi dengan 10 gambar yang didownload dari https://picsum.photos/, dimana setiap gambar akan didownload setiap 5 detik. Setiap gambar yang didownload akan diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss] dan gambar tersebut berbentuk persegi dengan ukuran (n%1000) + 50 pixel dimana n adalah detik Epoch Unix.

c. Setelah direktori telah terisi dengan 10 gambar, program tersebut akan membuat sebuah file “status.txt”, dimana didalamnya berisi pesan “Download Success” yang terenkripsi dengan teknik Caesar Cipher dan dengan shift 5. Caesar Cipher adalah Teknik enkripsi sederhana yang dimana dapat melakukan enkripsi string sesuai dengan shift/key yang kita tentukan. Misal huruf “A” akan dienkripsi dengan shift 4 maka akan menjadi “E”. Karena Ranora orangnya perfeksionis dan rapi, dia ingin setelah file tersebut dibuat, direktori akan di zip dan direktori akan didelete, sehingga menyisakan hanya file zip saja.

d. Untuk mempermudah pengendalian program, pembimbing magang Ranora ingin program tersebut akan men-generate sebuah program “Killer” yang executable, dimana program tersebut akan menterminasi semua proses program yang sedang berjalan dan akan menghapus dirinya sendiri setelah program dijalankan. Karena Ranora menyukai sesuatu hal yang baru, maka Ranora memiliki ide untuk program “Killer” yang dibuat nantinya harus merupakan program bash.

e. Pembimbing magang Ranora juga ingin nantinya program utama yang dibuat Ranora dapat dijalankan di dalam dua mode. Untuk mengaktifkan mode pertama, program harus dijalankan dengan argumen -z, dan Ketika dijalankan dalam mode pertama, program utama akan langsung menghentikan semua operasinya Ketika program Killer dijalankan. Sedangkan untuk mengaktifkan mode kedua, program harus dijalankan dengan argumen -x, dan Ketika dijalankan dalam mode kedua, program utama akan berhenti namun membiarkan proses di setiap direktori yang masih berjalan hingga selesai (Direktori yang sudah dibuat akan mendownload gambar sampai selesai dan membuat file txt, lalu zip dan delete direktori).
