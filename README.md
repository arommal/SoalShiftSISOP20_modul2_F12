# SoalShiftSISOPmodul2_F12

1. Pada soal ini, program diminta untuk dapat menerima argumen, sehingga kita memasukan parameter pada int main yaitu
```int main(int argc, char *argv[])```
Program diminta untuk mengeluarkan pesan jika argumnen yang dimasukkan salah, sehingga kami menggunakan variabel flag untuk melakukanm pengecekkan untuk setiap argumen dengan 
```C
if ((strcmp(argv[1],"*") == 0 || (atoi(argv[1])>=0 && atoi(argv[1])<=59 && argv[1][0] >= '0' && argv[1][0] <= '5' )) &&
      (strcmp(argv[2],"*") == 0 || (atoi(argv[2])>=0 && atoi(argv[2])<=59 && argv[2][0] >= '0' && argv[3][0] <= '5' )) &&
      (strcmp(argv[3],"*") == 0 || (atoi(argv[3])>=0 && atoi(argv[3])<=23 && argv[3][0] >= '0' && argv[2][0] <= '2' ))
      ) flag = 1;
```
flag akan bernilai 1 jika kriteria diatas memenuhi. Kemudian dilakukan pengecekkan nilai flag, jika bernilai 0 maka program akan mengeluarkan pesan ``Error, argumen yang diberikan tidak sesuai``
Pada poin d, program diminta untuk berjalan dibackground, sehingga kita menggunakan template daemon.
kita kemudian menyimpan nilai dari waktu yang sekarang kedalam variabel dan mengkonversinya menjadi string agar bisa melakukan strcmp antara argumen dan waktu yang sekarang
```C
        //pengambilan waktu skarang
        time_t now;
        struct tm *tm_struct;
        now = time(NULL);
        tm_struct=localtime(&now);
        //melakukan penyimpanan ke variabel
        int hour = tm_struct->tm_hour;
        int minute = tm_struct->tm_min;
        int second = tm_struct->tm_sec;
        //melakukan konversi ke string
        char sh[3], sm[3], ss[3];
```
Kemudian kita lakukan pengecekan untuk jam, menit, dan detik. Ketika waktu yang sekarng sudah sama dengan waktu yang diinginkan maka file sh yang dimasukkan pada argumen ke-4 akan dijalankan
```C
      if (strcmp(argv[3],"*") == 0|| strcmp(argv[3],sh)== 0) {				//pengecekkan jam
            if (strcmp(argv[2],"*")  == 0|| strcmp(argv[2],sm)== 0) {		//pengecekkan menit
                if (strcmp(argv[1],"*")  == 0|| strcmp(argv[1],ss) == 0){	//pengecekkan jam
                    if (fork()==0){
                      char *arg[] = {"bash", argv[4], NULL};				
                      execv("/bin/sh", arg);								//pengeksekusian file pada argumen ke-4
                    }
                }
            }
        }
```
Perlu diingat bahwa * adalah any value, sehingga pengecekkan karakter bintang juga dilakukan, Kemudian kita melakukan sleep selama 1 sekon dan program selseai.

2. Pada soal ini, terdapat 2 fungsi, yaitu fungsi `main` dan fungsi `generateKillerProgram`, yang akan membuat program `killerprogram.sh` untuk menterminasi program utama. Di dalam fungsi `main`, terdapat template implementasi Daemon process. Terdapat tambahan penentuan mode program yang dilakukan melalui pengecekan jumlah argumen dan isi argumen saat menjalankan program
```C
if(argc != 2){                      // pengecekan validitas argumen
      exit(EXIT_FAILURE);
}

generateKillerProgram(argv[1]);
```

Setelah itu, di dalam `while(1)` terdapat isi program utama. Dimulai dengan pendefinisian nama folder sesuai dengan format timestamp yang diinstruksikan sebelum pembuatan child process baru. Pengambilan format timestamp menggunakan fungsi `time()` dan `localtime()` untuk menyesuaikan dengan pengaturan waktu sistem.
```C
time_t t = time(NULL);
struct tm *tm = localtime(&t);
strftime(folder, 25, "%Y-%m-%d_%X", tm);
```

Lalu, dilakukan pembuatan child process pertama yang diikuti dengan `execv()` pembuatan folder dengan format nama yang sudah dibuat di atas.
```C
n1 = fork();
if(n1 < 0){
     exit(EXIT_FAILURE);
}
if(n1 == 0){
     char *arg1[] = {"mkdir", "-p", folder, NULL};
     execv("/bin/mkdir", arg1);
}
while(wait(&status) > 0);
```

Setelah folder terbentuk, diperlukan pembuatan child process baru untuk looping proses download file dari web serta zip dan hapus folder original. Sedangkan, proses download file dilakukan di child process yang baru lagi agar bisa berjalan paralel dengan proses lainnya.
Di dalam looping proses download, dilakukan pendefinisian nama file dengan cara yang sama dengan penamaan folder sebelumnya. Link download ditentukan sesuai instruksi dari soal. Setelah itu di dalam looping dibuat child process baru untuk pen-download-an dan penamaan file. Proses looping diberi jeda waktu selama 5 detik menggunakan fungsi `sleep(5)`. Proses zip dilakukan bersamaan dengan penghapusan folder asli menggunakan argumen `-rm` saat melakukan zipping. Kesatuan proses di atas dilakukan dengan jeda waktu 30 detik dengan fungsi `sleep(30)`.
```C
n2 = fork();

if(n2 == 0){
     int i = 0;
     char dl[100], file[25];
     for(;i<20;i++){
          time_t t2;
          time(&t2);
          struct tm *tm2 = localtime(&t2);
          strftime(file, 25, "%Y-%m-%d_%X", tm2);
          sprintf(dl, "https://picsum.photos/%ld", ((t2%1000)+100));

          n3 = fork();

          if(n3 == 0){
               char *arg2[] = {"wget", "-O", file, dl, NULL};
               chdir(folder);
               execl("/usr/bin/wget", "wget", "-O", file, dl, NULL);
          }
          sleep(5);
      }
            
      char zipname[100];
      sprintf(zipname, "%s.zip", folder);
      char *arg3[] = {"zip", "-rm", zipname, folder, NULL};
      execv("/usr/bin/zip", arg3);
  
}
sleep(30);
```

Untuk fungsi `generateKillerProgram`, diperlukan satu child process untuk pengeksekusian program yang telah dibuat. Ada dua kemungkinan isi program `killerprogram.sh`, yaitu:
- Jika argumen yang dimasukkan saat menjalankan program adalah "-a", terminasi semua proses yang berjalan
`killall -9 soal2 rm $0`

- Jika argumen yang dimasukkan saat menjalankan program adalah "-b", hanya terminasi parent process, jika masih ada child process yang berjalan, ditunggu hingga selesai
`kill getpid() rm $0`

Program ini dijalankan melalui shell script yang pengisiannya dilakukan dengan fungsi `fprintf()` ke file `killerprogram.sh`.
```C
if(strcmp(mode, "-a") == 0){
    fprintf(program, "#!/bin/bash\nkillall -9 soal2\nrm \"$0\"");
}else if(strcmp(mode, "-b") == 0){
    fprintf(program, "#!/bin/bash\nkill %d\nrm \"$0\"", getpid());
}
```

Setelah file tersebut berisi perintah yang sesuai, dilakukan pembuatan child process yang membolehkan kita mengeksekusi file tersebut di terminal.
```C
n1 = fork();

if(n1 == 0){
    char *arg1[] = {"chmod", "+x", "killerprogram.sh", NULL};
    execv("/bin/chmod", arg1);
}
```

3. Pada soal ini, kita akan membutuhkan banyak child process, pertama kita melakukan fork dengan ``pid = fork()`` dan kemudian kita menjalankan 
```C
    if (pid == 0) { 
        // First 
        // printf("1\n");
        char *argv[] = {"mkdir", "indomie", "sedaap", NULL};  
        execv("/bin/mkdir", argv);
    } 
```
yaitu melakukan pembuatan directory indomie dan sedaap kemudian kita melakukan wait dan melakukan fork lagi.
```C
    else { 
        while ((wait(&status))>0);
        pid1 = fork(); 
        if (pid1 == 0) { 
            // // second
            // printf("2\n");
            char *argv[] = {"unzip", "jpg.zip", NULL}; 
            execv("/usr/bin/unzip", argv);
        } 
```
 didalam child process ini, kita melakukan unzip pada `jpg.zip` yang terletak pada directory yang sama, dan melakukan execv. Kemudian kita melakukan wait dan fork lagi.
 ```C
         else { 
            while ((wait(&status))>0);
            pid2 = fork(); 
            if (pid2 == 0) { 
                // third
                // printf("3\n");

                char *argv[] = {"find", "/home/insanity/Modul2/jpg/.","-type","d", "-exec", "mv", "{}", "/home/insanity/Modul2/indomie",";" ,NULL};
                execv("/usr/bin/find", argv);
            } 
```
didalam child process ini, kita menggunakan find dengan ``-type d`` yaitu untuk menemukan semua file dengan tipe folder, dan melakukan ``-exec mv`` untuk menggunakan perintah move yaitu memindahkan semua file yang telah di find sebelumnya kedalam folder indomie yang telah dibuat sebelumnya. Kemudian kita melakukan wait dan fork lagi.
```C
            else { 
              while ((wait(&status))>0);
              pid3 = fork();
              if (pid3 == 0){
                // printf("4\n");
                char *argv[] = {"find","/home/insanity/Modul2/jpg/.", "-type", "f","-exec","mv","{}","/home/insanity/Modul2/sedaap",";", NULL}; 
                execv("/usr/bin/find", argv);
              }
```
Sama halnya dengan child sebelumnya, pada child process kali ini, kita melakukan find kembali, namun dengan tipe yang berbeda, yaitu f, yang berarti file, kemudian melakukan ``-exec mv`` ntuk menggunakan perintah move yaitu memindahkan semua file yang telah di find sebelumnya kedalam folder sedaap yang telah dibuat sebelumnya. Kemudian kita melakukan wait dan fork lagi.
```C
              else{
                    while ((wait(&status))>0);
                    pid4 = fork();
                    if (pid4 == 0){
                        //fifth
                        // printf("5\n");
                        char *argv[] = {"find", "/home/insanity/Modul2/indomie/.","-mindepth","1","-maxdepth","1","-type","d", "-exec", "touch", "{}/coba1.txt ",";", NULL}; 
                        execv("/usr/bin/find", argv);
                    }
```
pada child process kali ini, kita melakukan find lagi, namun kita menggunakan ``-mindpeth 1 -maxdepth 1 -type d`` yaitu kita hanya ingin kedalamannya 1, dan file yang kita find haruslah folder, kemudian kita menggunakan ``-exec touch {}/coba1.txt ;`` untuk membuat file ``coba1.txt`` untuk setiap folder yang telah kita find kemudian kita melakukan wait.
```C
                    else{
                        while ((wait(&status))>0);
                        sleep(3);
                        // printf("6\n");
                        char *argv[] = {"find", "/home/insanity/Modul2/indomie/.","-mindepth","1","-maxdepth","1","-type","d", "-exec", "touch", "{}/coba2.txt ",";", NULL}; 
                        execv("/usr/bin/find", argv);
                    }
```
pada Parent process ini, kita melakukan sleep terlebih dahulu selama 3 detik sebagaimana permintaan soal, kemudian kita melakukan hal yang sama pada childprocess diatas, yaitu membuat file coba2.txt untuk setiap folder.
