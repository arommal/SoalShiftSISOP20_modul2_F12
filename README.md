# SoalShiftSISOPmodul2_F12

1. Pada soal ini, program diminta untuk dapat menerima argumen, sehingga kita memasukan parameter pada int main yaitu
```int main(int argc, char *argv[])```
Program diminta untuk mengeluarkan pesan jika argumnen yang dimasukkan salah, sehingga kami menggunakan variabel flag untuk melakukanm pengecekkan untuk setiap argumen dengan 
```
if ((strcmp(argv[1],"*") == 0 || (atoi(argv[1])>=0 && atoi(argv[1])<=59 && argv[1][0] >= '0' && argv[1][0] <= '5' )) &&
      (strcmp(argv[2],"*") == 0 || (atoi(argv[2])>=0 && atoi(argv[2])<=59 && argv[2][0] >= '0' && argv[3][0] <= '5' )) &&
      (strcmp(argv[3],"*") == 0 || (atoi(argv[3])>=0 && atoi(argv[3])<=23 && argv[3][0] >= '0' && argv[2][0] <= '2' ))
      ) flag = 1;
```
flag akan bernilai 1 jika kriteria diatas memenuhi. Kemudian dilakukan pengecekkan nilai flag, jika bernilai 0 maka program akan mengeluarkan pesan ``Error, argumen yang diberikan tidak sesuai``
Pada poin d, program diminta untuk berjalan dibackground, sehingga kita menggunakan template daemon.
kita kemudian menyimpan nilai dari waktu yang sekarang kedalam variabel dan mengkonversinya menjadi string agar bisa melakukan strcmp antara argumen dan waktu yang sekarang
```
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
```
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


3. Pada soal ini, kita akan membutuhkan banyak child process, pertama kita melakukan fork dengan ``pid = fork();`` dan kemudian kita menjalankan 
```
    if (pid == 0) { 
        // First 
        // printf("1\n");
        char *argv[] = {"mkdir", "indomie", "sedaap", NULL};  
        execv("/bin/mkdir", argv);
    } 
```
yaitu melakukan pembuatan directory indomie dan sedaap kemudian kita melakukan wait dan melakukan fork lagi.
```
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
 ```
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
```
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
```
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
```
                    else{
                        while ((wait(&status))>0);
                        sleep(3);
                        // printf("6\n");
                        char *argv[] = {"find", "/home/insanity/Modul2/indomie/.","-mindepth","1","-maxdepth","1","-type","d", "-exec", "touch", "{}/coba2.txt ",";", NULL}; 
                        execv("/usr/bin/find", argv);
                    }
```
pada Parent process ini, kita melakukan sleep terlebih dahulu selama 3 detik sebagaimana permintaan soal, kemudian kita melakukan hal yang sama pada childprocess diatas, yaitu membuat file coba2.txt untuk setiap folder.
