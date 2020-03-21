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
