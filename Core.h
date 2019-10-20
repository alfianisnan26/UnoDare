#ifndef _CORE_H
#define _CORE_H

#ifndef _DEFINITION_H
#define _DEFINITION_H

//ERROR BYPASS DEFINE
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WIN32_WINNT 0x0510
#define CNULL cursor(PNULL,PNULL)
#define LUASOCKET_INET_PTON

//COLOR DEFINITION
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define LLBLUE 3
#define RED 4
#define PURPLE 5
#define YELLOW 6
#define LGREY 7
#define GREY 8
#define LBLUE 9
#define LGREEN 10
#define LLLBLUE 11
#define PINK 12
#define LPURPLE 13
#define LYELLOW 14
#define WHITE 15

//GETCH BUTTON
#define ENTER 13
#define ESCAPE 27
#define UP_ARROW 72
#define RIGHT_ARROW 77
#define LEFT_ARROW 75
#define DOWN_ARROW 80
#define BACKSPACE 8
#define SPACE 32
#define F1_BUTTON 59
#define DELETE_BUTTON 83
#define ALT 224

//NAVIGATION
#define NAV_VERTICAL 0
#define NAV_HORIZONTAL 1
#define NAV_ENTER -1
#define NAV_ESCAPE -2

//ROLES & STATE
#define AS_SERVER 0
#define AS_CLIENT 1
#define PLAY_GAME 0xFFFF
#define PLAYER_TIMEOUT 0xF001
#define PLAYER_NEXT 0xF0F1
#define PLAYER_DRAW_CARDS 0xF002
#define PLAYER_DROP_CARDS 0xF003
#define PLAYER_UNO 0xF004
#define PLAYER_ESCAPE 0xF005
#define STATE_IDLE 0xF000
#define WINDOW_GAME_EXIT -1
#define ZBUF sizeof(d_Sock)
#define SZE sizeof(d_buf)
#define GAME_OVER 0xFF33
#define UPDATE_DATA 41
#define DROP_CLIENT 42
#define Socket_Terminate 43
#define UPDATE_GAME_PLAY 44
#define UPDATE_OTHER_PLAYER 45
#define UPDATE_PCOUNT 46
#define INITIAL_DATA 47
#define UPDATE_USER 48
#define OTHER_USER_JOIN 49
#define SOCK_NOSERVER 0
#define SOCK_INITFAIL 1
#define SOCK_ERRADDSK 2
#define SOCK_SUCCESSINIT 3
#define SOCK_SERVEROFFLINE 4
#define SOCK_BINDERROR 5
#define USER_ADD 1
#define USER_DROP 2
#define USER_DROP_ALL 8
#define USER_DROP_BY_ID 9
#define USER_UPDATE 3
#define UPDATE_IP 1
#define UPDATE_NAME 2
#define UPDATE_TIME 3
#define UPDATE_ADD_CARDS 4
#define UPDATE_DROP_CARDS 5
#define UPDATE_SORT_CARDS 6
#define UPDATE_GROUP_CARDS 7
#define UPDATE_LAST3_CARDS 11
#define SEND_BROADCAST 100
#define TIMEOUT 10;

//INCLUDE LIBRARY
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <WS2tcpip.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <WinSock2.h>

//GLOBAL VARIABLE

int ipSelect;
char ipAddress[10][32];
char hostName[32];
char servName[32];
char dareRules[10][128];
char dareCount;
char ipServer[32];
const int portSet = 54000;

typedef struct clock_str {
	char minCh[2];
	char secCh[2];
	char clkCh[5];
	int inSec;
	int min;
	int sec;
} CLKSTR;

typedef struct cards_struct {
	int ID;
	int color;
	int color_d;
	char dir[8];
	char name_a[4];
	char name[17];
}CARDS;

typedef struct dl3 {
	CARDS cards;
	int index;
}d_l3;

typedef struct data_pass {
	int pCount;
	int myID;
	char myIP[32];
	int turn;
	bool winState;
	CLKSTR elapsed;
	bool Update;
	CARDS allCards[121];
	d_l3 Last3[4];
	int cardLeft;
	int state;
	int inverse;
}d_Pass;

typedef struct init_struct {
	char ServerIP[32];
	char ServerName[32];
	char ClientIP[32];
	char ClientName[32];
	int ClientID;
}IPID;

typedef struct player_struct {
	int ID;
	char name[32];
	char ip[32];
	CARDS cards[40];
	int cardCount;
	time_t signInTime;
	int lives;
}p_Uno;

typedef struct p_th {
	SOCKET sock;
	struct sockaddr_in sockaddr;
	HANDLE hThread;
	HANDLE rxThread;
	HANDLE txThread;
}pThr;

typedef struct data_play {
	int state;
	int update;
	fd_set *master;
}d_Play;

typedef struct data_buf {
	d_l3 Last3[3];
	p_Uno pUno;
	int turn;
	int state;
	int data;
}d_buf;

d_buf dBuf;
pThr pSock = { 0 };
d_Play dPlay;
p_Uno pUno[10] = { 0 };
p_Uno meOnly = { 0 };
d_Pass dPass = { 0 };

#include "Frontend.h"

//PRAGMA
#pragma comment (lib, "ws2_32.lib")
#endif

 /*LOW LEVEL BACKEND*/
int initSocket() {
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsok = WSAStartup(ver, &wsData);
	if (wsok != 0) {
		printf("Can't initialize wsok!\n");
		system("pause");
		return -1;
	}

	//myDebug
	return 0;
}

int state = FALSE;
DEVMODE devmodeBak;
void setRes(const int pX, const int pY) {
	if (state == TRUE) {
		ChangeDisplaySettings(&devmodeBak, 0);
		return 0;
	}
	state = TRUE;
	DEVMODE devmode;
	devmode.dmPelsWidth = pX;
	devmode.dmPelsHeight = pY;
	devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	devmode.dmSize = sizeof(DEVMODE);
	ChangeDisplaySettings(&devmode, 0);
	return 0;
}

int getIPAddress()
{
	initSocket();
	int ipCount = 0;
	char ac[80];
	ZeroMemory(ipAddress, sizeof(ipAddress));
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		strcat(ipAddress[0],"Error when getting local host name.");
		return 1;
	}
	struct hostent* phe = gethostbyname(ac);
	if (phe == 0) {
		strcat(ipAddress[0],"Bad host lookup.");
		return 1;
	}
	strcat(ipAddress[0], "127.0.0.1");
	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		strcat(ipAddress[i + 1], inet_ntoa(addr));
		ipCount++;
	}
	WSACleanup();
	return ipCount;
}

int arrowNav(int count, int min, int max, int direction) {
	int r = _getch();
	if (r == 224) {
		if (direction == NAV_VERTICAL) {
			switch (_getch()) {
			case UP_ARROW:
				count--;
				if (count < min)count = max;
				break;
			case LEFT_ARROW:
				count = NAV_ESCAPE;
				break;
			case RIGHT_ARROW:
				count = NAV_ENTER;
				break;
			case DOWN_ARROW:
				count++;
				if (count > max)count = min;
				break;
			}
		}
		else if (direction == NAV_HORIZONTAL) {
			switch (_getch()) {
			case UP_ARROW:
				count = NAV_ENTER;
				break;
			case LEFT_ARROW:
				count--;
				if (count < min)count = max;
				break;
			case RIGHT_ARROW:
				count++;
				if (count > max)count = min;
				break;
			case DOWN_ARROW:
				count = NAV_ESCAPE;
				break;
			}
		}
	}else count = -1 * r;
	return count;
}

int scanSuper(COORD curs, char stringArr[], int stringLen,char lastCall) {
	COORD stdCurs = curs;
	setc(curs);
	char myArr[256];
	ZeroMemory(myArr, sizeof(myArr));
	if (stringLen < strlen(myArr)) ZeroMemory(stringArr, sizeof(stringArr));
	strcpy(myArr, stringArr);
	cursorvisibility(TRUE);
	printf(stringArr);
	int pos = strlen(stringArr);
	if (lastCall > 0) {
		myArr[pos] = lastCall;
		putchar(lastCall);
		pos++;
	}
	while (TRUE) {
		int r = _getch();
		if (r == 224) {
			int getVal = _getch();
			if (getVal == LEFT_ARROW) {
				pos = (pos <= 0) ? strlen(myArr) : pos-1;
				curs.X = stdCurs.X + pos;
				setc(curs);
			}
			else if (getVal == RIGHT_ARROW) {
				pos = (pos < strlen(myArr)) ? pos + 1 : 0;
				curs.X = stdCurs.X + pos;
				setc(curs);
			}
			else if (getVal == UP_ARROW || getVal == DOWN_ARROW) {
				ZeroMemory(stringArr, sizeof(stringArr));
				for (int i = 0; i < strlen(myArr); i++) {
					stringArr[i] = myArr[i];
				}
				cursorvisibility(FALSE);
				if (getVal == UP_ARROW) return -1;
				else if (getVal == DOWN_ARROW) return 1;
			}
		}
		else if (r >= ' ' && r <= '~') {
			setc(stdCurs);
			for (int i = 0; i < strlen(myArr); i++)printf(" ");
			setc(stdCurs);
			for (int i = strlen(myArr); i > pos; i--) {
				myArr[i] = myArr[i - 1];
			}
			myArr[pos] = r;
			pos++;
			printf(myArr);
			curs.X = stdCurs.X + pos;
			setc(curs);
			if (strlen(myArr) >= stringLen) {
				ZeroMemory(stringArr, sizeof(stringArr));
				for (int i = 0; i < strlen(myArr); i++) {
					stringArr[i] = myArr[i];
				}
				break;
			}
		}
		else if (r == BACKSPACE && pos > 0) {
			setc(stdCurs);
			for (int i = 0; i < strlen(myArr); i++)printf(" ");
			pos--;
			for (int a = pos; a <= strlen(myArr); a++) {
				myArr[a] = myArr[a + 1];
			}
			setc(stdCurs);
			printf(myArr);
			curs.X = stdCurs.X + pos;
			setc(curs);

		}
		if (r == ESCAPE) {
			setc(stdCurs);
			for (int i = 0; i < strlen(myArr); i++) printf(" ");
			setc(stdCurs);
			printf(stringArr);
			cursorvisibility(FALSE);
			return ESCAPE;
			break;
		}
		else  if (r == ENTER) {
			ZeroMemory(stringArr, sizeof(stringArr));
			strcpy(stringArr, myArr);
			cursorvisibility(FALSE);
			return ENTER;
			break;
		}
	}
}

int getch_nonblock() {
	if (_kbhit()) return _getch();
	else return -1;
}

p_Uno makeuser(char* ip, char* name, int myID) {
	p_Uno p;
	strcpy(p.ip, ip);
	strcpy(p.name, name);
	p.ID = myID;
	return p;
}

CARDS user_set(int *state, ...) {
	p_Uno usr;
	va_list arg;
	va_start(arg, state);
	if (state == USER_ADD) {
		if (dPass.pCount == 10) {
			state = -1;
			return;
		}
		usr = va_arg(arg, p_Uno);
		usr.cardCount = 0;
		ZeroMemory(usr.cards, sizeof(usr.cards));
		time(&usr.signInTime);
		pUno[dPass.pCount] = usr;
		dPass.pCount++;
	}
	else if (state == USER_DROP) {
		if ((dPass.pCount - 1) == 1) {
			state = -1;
			return;
		}
		for (int a = va_arg(arg, int); a < dPass.pCount; a++) pUno[a] = pUno[a + 1];
		dPass.pCount--;
	}
	else if (state == USER_DROP_ALL) {
		ZeroMemory(pUno, sizeof(pUno));
		dPass.pCount = 0;
	}
	else if (state == USER_DROP_BY_ID) {
		int ID = va_arg(arg, int);
		int count = 0;
		for (int a = 0; a <= 10; a++) {
			count = a;
			if (count == 10) return;
			if (pUno[a].ID == ID) break;
		}
		for (int a = count; a < dPass.pCount; a++) pUno[a] = pUno[a + 1];
		dPass.pCount--;
	}
	else if (state == USER_UPDATE) {
		int ID = va_arg(arg, int);
		int st = va_arg(arg, int);
		if (st == UPDATE_IP) strcat(pUno[ID].ip, va_arg(arg, char));
		else if (st == UPDATE_NAME) strcat(pUno[ID].name, va_arg(arg, char));
		else if (st == UPDATE_TIME) time(&pUno[ID].signInTime);
		else if (st == UPDATE_ADD_CARDS) {
			pUno[ID].cards[pUno[ID].cardCount] = va_arg(arg, CARDS);
			pUno[ID].cardCount++;
		}
		else if (st == UPDATE_DROP_CARDS) {
			int num = va_arg(arg, int);
			CARDS cr = pUno[ID].cards[num];
			for (int a = num; a < pUno[ID].cardCount; a++) pUno[ID].cards[a] = pUno[ID].cards[a + 1];
			pUno[ID].cardCount--;
			return cr;
		}
		else if (st == UPDATE_GROUP_CARDS) {
			for (int a = 1; a < pUno[ID].cardCount; a++) {
				if (pUno[ID].cards[a].color < pUno[ID].cards[a - 1].color) {
					CARDS dump = pUno[ID].cards[a];
					pUno[ID].cards[a] = pUno[ID].cards[a - 1];
					pUno[ID].cards[a - 1] = dump;
				}
			}
		}
		else if (st == UPDATE_SORT_CARDS) {
			for (int a = 1; a < pUno[ID].cardCount; a++) {
				if (pUno[ID].cards[a].name_a[0] < pUno[ID].cards[a - 1].name_a[0]) {
					CARDS dump = pUno[ID].cards[a];
					pUno[ID].cards[a] = pUno[ID].cards[a - 1];
					pUno[ID].cards[a - 1] = dump;
				}
			}
		}
	}
	va_end(arg);
}

int getIndexNumber(int ID) {
	for (int a = 0; a < dPass.pCount; a++) {
		if (ID == pUno[a].ID) {
			return a;
		}
	}
}

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

#include "SocketH.h"

void draw(int size) {
	if (dPass.myID == AS_SERVER) {
		for (int a = 0; a < size; a++) {
			dPass.cardLeft--;
			meOnly.cards[meOnly.cardCount + a] = dPass.allCards[dPass.cardLeft];
			ZeroMemory(&dPass.allCards[dPass.cardLeft], sizeof(dPass.allCards[dPass.cardLeft]));
		}
		meOnly.cardCount += size;
		pUno[0].cardCount += size;
		d_buf buf;
		dPlay.update = UPDATE_PLAYER_CARD_COUNT;
		buf.state = UPDATE_PLAYER_CARD_COUNT;
		buf.data = 0;
		buf.pUno.cardCount = pUno[buf.data].cardCount;
		sendData(SEND_BROADCAST, buf);
		PlayGame(PLAYER_NEXT);
	}
	else {
		d_buf buf;
		buf.state = DRAW_CARDS;
		buf.data = size;
		sendData(pSock.sock, buf);
	}
	return;
}

int drop(bool boxState[],int qCount) {
	d_buf buf;
	//PINDAHKAN KARTU
	int count = 0;
	for (int a = 0; a < meOnly.cardCount; a++) {
		if (boxState[a] == TRUE) {
			buf.pUno.cards[count] = meOnly.cards[a];
			count++;
			if (count == qCount) break;
		}
	}
	//SUSUN ULANG KARTU SAYA
	count = 0;
	for (int a = 0; a < meOnly.cardCount; a++) {
		if (boxState[a] == TRUE) {
			count++;
		}
		meOnly.cards[a] = meOnly.cards[a + count];
		if (count == qCount) break;
	}
	meOnly.cardCount -= qCount;
	buf.pUno.cardCount = meOnly.cardCount;
	buf.data = getIndexNumber(dPass.myID);
	buf.turn = qCount;
	//KIRIM KARTU
	for (int a = 0; a < qCount; a++) {
		dPass.Last3[0] = dPass.Last3[1];
		dPass.Last3[1] = dPass.Last3[2];
		dPass.Last3[2].cards = buf.pUno.cards[a];
		dPass.Last3[2].index = getIndexNumber(dPass.myID);
		dPass.allCards[dPass.cardLeft] = buf.pUno.cards[a];
		dPass.cardLeft++;
	}
	for (int a = 0; a < 3; a++) {
		buf.Last3[a] = dPass.Last3[a];
	}
	buf.state = PLAYER_DROP_CARDS;
	if (dPass.myID == AS_SERVER) {
		pUno[0].cardCount = meOnly.cardCount;
		sendData(SEND_BROADCAST, buf);
		PlayGame(PLAYER_NEXT);
	}
	else {
		sendData(pSock.sock, buf);
	}
	return TRUE;
}

#include "DisplayConsole.h"

int WindowGamePlay() {
	clrs();
	dPass.turn = 0;
	dPass.winState = TRUE;
	CreateThread(NULL, 0, GamePlayDisp_thread, NULL, 0, NULL);
	clock_t sec = clock();
	dPass.elapsed.inSec = 60 * 20;
	Sleep(1000);
	while (1) {
		clock_t rec = clock();
		if (((double)rec >= (double)sec + 1000)) {
			sec = rec;
			dPass.elapsed.inSec--;
			dPass.elapsed = updateCLK(dPass.elapsed);
			if (dPass.elapsed.inSec == 0) {
				dPlay.state = GAME_OVER;
				break;
			}
		}
		else if (meOnly.cardCount > 40 || meOnly.cardCount == 0 || dPlay.state == GAME_OVER) {
			dPlay.state = GAME_OVER;
			if (dPass.myID == AS_SERVER) {
				d_buf buf;
				buf.state = GAME_OVER;
				sendData(SEND_BROADCAST, buf);
			}
			else {
				d_buf buf;
				buf.state = GAME_OVER;
				sendData(pSock.sock, buf);
			}
			break;
		}
		else if (meOnly.lives <= 0) {
			dPlay.state = GAME_OVER;
			break;
		}
		if (dPass.state == GAME_OVER) break;
	}
	Sleep(100);
	clrs();
	printbig(cursor(59, 20), RED, WHITE, "GAME OVER");
	setc(cursor(62, 26)); printf("ESCAPE UNTUK KELUAR | TOMBOL BEBAS UNTUK KEMBALI");
	TerminateThread(pSock.rxThread, NULL);
	TerminateThread(pSock.hThread, NULL);
	closesocket(pSock.sock);
	WSACleanup();
	user_set(USER_DROP_ALL);
	int x = _getch();
	if (x == ESCAPE) return WINDOW_GAME_EXIT;
	else return 0;
}

int randNumRange(int min, int max) {
	return (rand() %(max - min)) + min;
}

void InitDataBase() {
	ZeroMemory(dPass.allCards, sizeof(dPass.allCards));
	if (dPass.myID == AS_SERVER) {
		//Initializing Cardss
		int color[5] = { RED,YELLOW,GREEN,BLUE,BLACK };
		int color_d[5] = { PINK,LYELLOW,LGREEN,LBLUE,GREY };
		char name[18][17] = { "WILD","DRAW EMPAT KARTU","DARE CARDS","NOL","SWAP","REVERSE","DRAW DUA KARTU","BLOCK","SATU","DUA","TIGA","EMPAT","LIMA","ENAM","TUJUH","DELAPAN","SEMBILAN" };
		char name_a[18][4] = { "W","+4","!","0","S","R","+2","B","1","2","3","4","5","6","7","8","9" };
		int cName = -1;
		int cCol = -1;
		for (int r = 0; r < 120; r++) {
			if (r >= 0 && r < 20) {
				if (r % 4 == 0) cName++;
				if (r > 11) cCol = r % 4;
				else cCol = 4;
			}
			else if (r >= 20 && r < 120) {
				if (r % 8 == 0)cName++;
				if (r % 2 == 0) cCol = (r / 2) % 4;
			}
			dPass.allCards[r].color = color[cCol];
			dPass.allCards[r].color_d = color_d[cCol];
			strcpy(dPass.allCards[r].name, name[cName]);
			strcpy(dPass.allCards[r].name_a, name_a[cName]);
			dPass.allCards[r].ID = r;
			char dir[8] = "Data\\";
			strcat(dir, name_a[cName]);
			strcpy(dPass.allCards[r].dir, dir);
		}
		dPass.inverse = 1;
		dPass.cardLeft = 120;
		for (int a = 0; a < 240; a++) {
			int d = randNumRange(0, 60);
			int x = randNumRange(60, 120);
			CARDS dump = dPass.allCards[d];
			dPass.allCards[d] = dPass.allCards[x];
			dPass.allCards[x] = dump;
		}
		for (int r = 0; r < dPass.cardLeft; r++) {
		}
		dPass.turn = 0;
		int counter = 0;
		int last3Counter = 0;
		while (dPass.cardLeft != 120 - 3) {
			if (dPass.allCards[counter].name_a[0] >= '0' && dPass.allCards[counter].name_a[0] <= '9') {
				dPass.Last3[last3Counter].cards = dPass.allCards[counter];
				dPass.Last3[last3Counter].index = -1;
				dPass.allCards[counter].ID = -1;
				last3Counter++;
				dPass.cardLeft--;
			}
			counter++;
		}
		counter = 0;
		last3Counter = 1;
		while (last3Counter <= 3) {
			if (dPass.allCards[counter].ID == -1) {
				dPass.allCards[counter] = dPass.allCards[dPass.cardLeft - last3Counter + 3];
				ZeroMemory(&dPass.allCards[dPass.cardLeft - last3Counter + 3], sizeof(dPass.allCards[dPass.cardLeft - last3Counter + 3]));
				last3Counter++;
			}
			counter++;
		}
		for (int a = 0; a < dPass.pCount; a++) {
			ZeroMemory(&pUno[a].cards, sizeof(pUno[a].cards));
			for (int b = 0; b < 7; b++) {
				dPass.cardLeft--;
				pUno[a].cards[b] = dPass.allCards[dPass.cardLeft];
				ZeroMemory(&dPass.allCards[dPass.cardLeft], sizeof(dPass.allCards[dPass.cardLeft]));
			}
			pUno[a].cardCount = 7;
			pUno[a].lives = 3;
		}
		updateUserData();
		d_buf buf;
		buf.state = PLAY_GAME;
		buf.data = dPass.pCount;
		buf.turn = 0;
		for (int a = 0; a < 3; a++)buf.Last3[a] = dPass.Last3[a];
		sendData(SEND_BROADCAST, buf);
		meOnly = pUno[getIndexNumber(dPass.myID)];
		dPlay.state = PLAY_GAME;
		dPass.state = STATE_IDLE;
		return;
	}
	else {
		dPlay.state = PLAY_GAME;
		dPass.state = STATE_IDLE;
		return;
	}
}
#endif