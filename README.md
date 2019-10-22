# UnoDare!
Proyek Akhir Proglan UTS
- Nama Proyek	: UnoDare!
- Jurusan   	: Teknik Komputer
- Instansi  	: Universitas Indonesia
- Dibuat    	: Bulan Oktober 2019
- Kelompok  	:
	- Alfian Badrul Isnan
	- Idham Ramadhito
	- Ali Baghir

## Penjelasan Program
[Uno](https://id.wikipedia.org/wiki/UNO_(permainan))Dare! adalah permainan kartu uno tetapi ada sedikit perubahaan dalam uno dare.
Dalam permainan uno dare dapat dimainkan oleh 2 – 10 orang pemain, di dalam uno dare terdapat 108 kertu.peraturaan yang dipakai sama dengan permainan uno seperti biasanya, Yang berbeda pada uno dare yaitu deck uno dare terdapat empat kartu dare, kartu dare ini adalah sebuah special card yang mempunyai kelebihan khusus. Apabila anda terkena kartu dare, anda harus melakukan dare yang sudah disepakati Bersama. Masing-masing warna memliki satu angka 0, dua angka 1-9, dan dua simbol stop, draw two, reverse. 	

## Cara Bermain
Cara bermainnya yaitu :
1. Deck akan di kocok dan tiap pemain akan diberikan 7 kartu. 
2. Untuk memulai game ambil 1 kartu dari deck dan letakkan ditengah meja.
3. mainkan kartu dari tangan anda yang sesuai dengan warna/angka/simbol dengan kartu ditengah meja. Anda dapat memainkan lebih dari satu kartu ditangan jika memiliki kartu dengan angka/simbol yang sama. 
4. Jika tidak bisa mengeluarkan kartu dari tangan ambil kartu dari deck.
5. Perhatikan kartu aksi dan Wild card seperti :
    - Kartu Reverse : Membalikkan arah permainan
    - Kartu Skip : Melewatkan pemain setelah kartu stop dimainkan
    - Kartu +2  : Memberi pemain setelah kartu +2 dua kartu dari deck.
    - Kartu +4  : Memberi pemain setelah kartu 4 empat kartu dari deck dan yang mengeluarkan memilih warna yang ingin dimainkan
    - Kartu Mengganti Warna : Mengganti warna kartu yang akan dimainkan sesuai pilihan yang memainkan kartu
6. Keenam, jika kartu anda tersisa 1 ucapkan UNO!, jika orang lain yang mengucapkannya ambil 2 kartu dari deck
7. Ketujuh, mainkan kartu terakhir anda untuk memenangkan gamenya. Dan itu adalah cara untuk bermain UNO DARE.	

## Fitur

### User Friendly
Pada Aplikasi ini masih digunakan tampilan berupa console.

#### Tampilan Menu Awal
<img src="https://github.com/alfianisnan26/UnoDare/blob/master/Screenshoot/Screenshot%20(279).png" width="400">

#### Tampilan Buat/Gabung
##### Server
<img src="https://github.com/alfianisnan26/UnoDare/blob/master/Screenshoot/Screenshot%20(280).png" width="400">

##### Client
<img src="https://github.com/alfianisnan26/UnoDare/blob/master/Screenshoot/Screenshot%20(281).png" width="400">

#### Tampilan Waiting Room
##### Server
<img src="https://github.com/alfianisnan26/UnoDare/blob/master/Screenshoot/Screenshot%20(304).png" width="400">

##### Client
<img src="https://github.com/alfianisnan26/UnoDare/blob/master/Screenshoot/Screenshot%20(305).png" width="400">

#### Tampilan Gameplay
<img src="https://github.com/alfianisnan26/UnoDare/blob/master/Screenshoot/Screenshot%20(306).png" width="400">

### Multiplayer
Game ini telah dapat beroperasi dengan sistem Multi-pengguna melalui jaringan lokal (LAN ataupun WLAN) tetapi hanya dapat digunakan pada sistem operasi Windows saja.
Multiplayer ini terbentuk karena digunakannya Windows Socket API (dituliskan pada subtitle header).

## Download
  - [Executable File](https://bit.ly/UnoDareInstall)
  - [Developer Zip](https://drive.google.com/file/d/16Yno_sfLaXNF3nybCYHEOKRoyRZEJbev/view?usp=sharing)
  - [VS Full Project](https://drive.google.com/file/d/1jEwGagC7jjtW2Q_NOYGHzl4JbouXzLjX/view?usp=sharing)
  - [Documentation](https://drive.google.com/file/d/16XHLg1-NpKHxs3f3gMTngxebFeTofPQJ/view?usp=sharing)
  - [Github](https://github.com/alfianisnan26/UnoDare)

## Source
Program ini di buat dan di compile menggunakan IDE (Integrated Developement Environtment) [Visual Studio Community 2019](https://visualstudio.microsoft.com/vs/).

### Main Program
UnoDare.c
```c
int main(){
	...
	initDisplay();
	int menu = 0;
	while (menu >= 0) {
		menu = WindowMainMenu(menu);
		int pos;
		if (menu == 4) {
			pos = WindowExit();
			if (pos == 0) menu = -1;
			else if (pos == 1) break;
		}
		else if (menu == 0 || menu == 1) {
			dPass.myID = menu - 1;
			while (1) {
				initSocket();
				pos = (menu == 0)?WindowJoin():WindowCreate();
				if (pos == 1) {
					pos = WindowWaiting();
					if (pos == 1) {
						InitDataBase();
						pos = WindowGamePlay();
						if( pos == WINDOW_GAME_EXIT) return 0;
						else break;
					}
				}
				else break;
			}
		}
		else if (menu == 2) {
			WindowHelp();
		}
		else if (menu == 3) {
			WindowAbout();
		}
	}
	...
	return 0;
}
```


### Header
Header utama yang di include pada UnoDare.c
```c
#include "Core.h" //Modular Inti & Backend
```

Header yang di include pada Core.h
```c
#include <stdio.h> //Standard Input/Output Library in C Language
#include <string.h> //String Library
#include <stdarg.h> //Standard Arg Library
#include <stdbool.h> //Standard Boolean Library
#include <WS2tcpip.h> //Windows Socket 2 TCP/IP Library
#include <conio.h> //Conio Library
#include <Windows.h> //Windows API Library
#include <time.h> //Time Library
#include <WinSock2.h> //Windows Socket 2 Standard Library

#pragma comment (lib, "ws2_32.lib") //Diperlukan karena penggunaan WinSock2.h
```
Header Lokal di include pada Core.h
```c
#include "DisplayConsole.h" //berisi Modular High-End Frontend
#include "Frontend.h" //berisi Modular Low-End Frontend
#include "Socket.h" //berisi Modular untuk Passing data pada socket TCP/IP
```
#### Core.h
Pada Core.h dibuat variabel-variabel global untuk menyimpan data pemain secara lokal, bentuk data passing (berupa struct) dan ID setiap kartu.
  - Global Variable untuk Identitas & Status pemain
    ```c
    typedef struct player_struct {
      int ID;
      char name[32];
      char ip[32];
      CARDS cards[40];
      int cardCount;
      time_t signInTime;
      int lives;
    }p_Uno;
    
    p_Uno meOnly = { 0 }; //Definisi Global Variabel untuk menyimpan ID Lokal (ID Client di masing-masing Komputer)
    ```
  - Global Variabel untuk ID Kartu
    ```c
    typedef struct cards_struct {
      int ID;
      int color;
      int color_d;
      char dir[8];
      char name_a[4];
      char name[17];
    }CARDS;
    ```
  - Global Variabel untuk Passing Data pada Socket Programming
    ```c
    typedef struct data_buf {
      d_l3 Last3[3];
      p_Uno pUno;
      int turn;
      int state;
      int data;
    }d_buf;
    ```

Dalam Core.h dibuat modular-modular penting untuk menjalankan aplikasi.
```c
int initSocket(); //Inisiasi Socket untuk WinSock
int getIPAddress(); //Modular untuk mendapatkan IP Server
p_Uno makeuser(char* ip, char* name, int myID); //Modular untuk membentuk ID Pemain (pada Struct p_Uno)
void InitDataBase(); //Modular Untuk menginisiasi keadaan awal pemain dan mengacak susunan kartu
int WindowGamePlay(){ //Modular berupa fungsi untuk menjalankan Tampilan GamePlay (Thread) dan Socket (Thread)
  ...
  CreateThread(NULL, 0, GamePlayDisp_thread, NULL, 0, NULL);
  ...
  while(1){
    ...
    //Fungsi menghitung waktu
    ...
  }
  ...
  TerminateThread(pSock.rxThread, NULL);
  TerminateThread(pSock.hThread, NULL);
  closesocket(pSock.sock);
  WSACleanup();
  ...
}
```

Modular Fungsi untuk Sortir Kartu
```c
#define CARD_SORT_BY_COLOR 1
#define CARD_SORT_BY_CHAR 2
void cardSort(bool * boxState, int state) {
	for (int i = 0; i < meOnly.cardCount - 1; i++) {
		for (int j = 0; j < meOnly.cardCount - i - 1; j++) {
			if ((state == CARD_SORT_BY_CHAR) ? (meOnly.cards[j].name_a[0] > meOnly.cards[j + 1].name_a[0]) : (meOnly.cards[j].color < meOnly.cards[j + 1].color)) {
				CARDS dump = meOnly.cards[j];
				bool dumpId = boxState[j];
				meOnly.cards[j] = meOnly.cards[j + 1];
				boxState[j] = boxState[j + 1];
				meOnly.cards[j + 1] = dump;
				boxState[j + 1] = dumpId;
			}
		}
	}
}
```

Contoh sortir kartu dengan perintah (state) CARD_SORT_BY_CHAR

<img src="https://github.com/alfianisnan26/UnoDare/blob/master/Screenshoot/Undare4.png" width="300">

#### Frontend.h
Beberapa modular pada Frontend.h adalah sebagai berikut:
```c
void printSuper(COORD cursor, int ForgC, int BackC, const char* val, ...);

typedef struct boxID_struct { //Struct untuk ID Box
	COORD curs;
	int sizeX;
	int sizeY;
	int type;
	int color;
} boxID;

boxID makebox(COORD curs, int sizeX, int sizeY, int color, int type);
boxID box(int state, boxID box_ID,...);
```

Penggunaan printImageAscii()
```c
void printImageAscii(COORD cursor_val, const char* filename, char x, int color, ...);
```
<img src="https://github.com/alfianisnan26/UnoDare/blob/master/Screenshoot/Undare.png" width="300">

Penggunaan modular printBig()
```c
void printbig(COORD curs, int ForgC, int BackC, char* strg)
```
<img src="https://github.com/alfianisnan26/UnoDare/blob/master/Screenshoot/Undare1.png" width="500">

#### Socket.h
Dalam header ini dibuat modular dan fungsi untuk melakukan perpindahan data antara client dan server. Selain itu dibentuk pula Thread untuk menerima data.
Pembuatan Thread untuk menerima data dibuat karena fungsi recv akan tertahan saat tidak ada data yang diterima sehingga aplikasi dapat freeze (Tidak Looping).

Modular-modular Passing data
```c
void broadcast(fd_set master, char* buf, int len); //Untuk Passing data dari server kesemua Client
void sendData(int toID, d_buf buf); //Untuk mengirimkan data ke Server/Client (dengan toID)
d_buf getData(); //Untuk mendapatkan data dari struct d_buf

void PlayGame(int state); //Pengiriman data dengan Kondisi Tertentu

//Thread For Recieving Data
DWORD WINAPI ServerThread(void); //Untuk Server
DWORD WINAPI ClientThread(void); //Untuk Client

int socketInit(int state); //Inisiasi Socket
```
Fungsi dalam modular socketInit()
```c
int socketInit(int state) {
	user_set(USER_DROP_ALL);
	if (state == AS_SERVER) {
		//Create Socket
		pSock.sock = socket(AF_INET, SOCK_STREAM, 0);
		if (pSock.sock == INVALID_SOCKET) {
			return SOCK_ERRADDSK;
		}
		char char_array[16];
		strcpy(char_array, ipServer);

		struct sockaddr_in localaddr;
		localaddr.sin_family = AF_INET;
		localaddr.sin_addr.s_addr = inet_addr(char_array);
		localaddr.sin_port = htons(portSet);  // Any local port will do
		bind(pSock.sock, (struct sockaddr*) & localaddr, sizeof(localaddr));

		char* ip = inet_ntoa(localaddr.sin_addr);
		user_set(USER_ADD, makeuser(ipServer, hostName, 0));
		strcpy(dPass.myIP, ipServer);
		dPass.myID = 0;
		listen(pSock.sock, SOMAXCONN);
		pSock.rxThread = CreateThread(NULL, 0, ServerThread, NULL, 0, NULL);
	}
	else if (state == AS_CLIENT) {
		// Initialize WinSock

		// Create socket
		pSock.sock = socket(AF_INET, SOCK_STREAM, 0);
		if (pSock.sock == INVALID_SOCKET)
		{
			return SOCK_INITFAIL;
		}

		// Fill in a hint structure
		struct sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(portSet);
		//inet_pton(AF_INET, ipServer, &hint.sin_addr);
		hint.sin_addr.s_addr = inet_addr(ipServer);
		// Connect to server

		int connResult = connect(pSock.sock, (struct sockaddr*) & hint, sizeof(hint));
		if (connResult == SOCKET_ERROR)
		{
			return SOCK_NOSERVER;
		}
		IPID ips = { 0 };
		char sbuf[sizeof(ips)];
		recv(pSock.sock, sbuf, sizeof(ips), 0);
		memcpy(&ips, sbuf, sizeof(ips));
		strcpy(dPass.myIP, ips.ClientIP);
		strcpy(servName, ips.ServerName);
		dPass.myID = ips.ClientID;
		strcpy(ips.ClientName, hostName);
		ZeroMemory(sbuf, sizeof(sbuf));
		memcpy(sbuf, &ips, sizeof(ips));
		send(pSock.sock, sbuf, sizeof(ips), 0);
		pSock.rxThread = CreateThread(NULL, 0, ClientThread, NULL, 0, NULL);
	}
	return SOCK_SUCCESSINIT;
}
```

#### DisplayConsole.h
Pada display console, dibuat modular untuk menampilkan tampilan pada setiap menu.
```c
void initDisplay(); //Inisiasi Display seperti Fullscreen, Warna Global, dsb.

int WindowMainMenu(int position);
int WindowExit();
int WindowCreate();
int WindowJoin();
int WindowWaiting();
int WindowHelp();
int WindowAbout();

//Thread
DWORD WINAPI GamePlayDisp_thread();
```

## Referensi
  - [Socket Programming](https://www.geeksforgeeks.org/socket-programming-cc/)
  - [Windows Socket API](https://docs.microsoft.com/en-us/windows/win32/winsock/windows-sockets-start-page-2)
  - [Winsock.h Documentation](https://docs.microsoft.com/en-us/windows/win32/api/winsock/index)
  - [Windows Console API](https://docs.microsoft.com/en-us/windows/console/)
