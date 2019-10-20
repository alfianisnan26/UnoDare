#ifndef  _DISPLAYCONSOLE_H
#define _DISPLAYCONSOLE_H

int win = 0;
void initDisplay() {
	get_handle();
	if (win == 0) windowMode(FULLSCREEN);
	setscreenbuffersize(168,48);
	setsystemcolor(BLACK, WHITE);
	cursorvisibility(FALSE);
}

int WindowMainMenu(int position) {
	clrs();
	COORD myC = {(168 / 2) - 25 ,10};
	printImageAscii(myC, "data\\logo.txt", 'U', RED, 'N', YELLOW, 'O', GREEN, 'd', BLUE);
	boxID myBox[5];
	char myMenu[6][8] = {
		"GABUNG",
		" BUAT",
		"BANTUAN",
		"TENTANG",
		"KELUAR"
	};
	for (int i = 0; i < 5; i++) {
		myBox[i] = box(DRAW, makebox(cursor(45 + 16 * i, 32), 14, 7, (i == position) ? GREY : LGREY, BOX_FILL));
		printSuper(cursor(49 + 16 * i, 35), (i == position) ? WHITE : BLACK, (i == position) ? GREY : LGREY, "%s", myMenu[i]);
	}
	while (TRUE) {
		int r = _getch();
		int pos_old = position;
		int state = FALSE;
		if (r == RIGHT_ARROW) {
			position++;
			if (position >= 5) position = 0;
			state = TRUE;
		}
		else if (r == LEFT_ARROW) {
			position--;
			if (position < 0)position = 4;
			state = TRUE;
		}
		else if (r == ESCAPE) {
			position = 4;
			state = TRUE;
			break;
		}
		else if (r >= 49 && r <= 53) {
			position = r - 49;
			state = TRUE;
		}
		else if (r == ENTER) break;
		if (state == TRUE) {
			box(UPDATE, myBox[pos_old], BOX_SETCOLOR, LGREY);
			printSuper(cursor(49 + 16 * pos_old, 35), BLACK, LGREY, "%s", myMenu[pos_old]);
			box(UPDATE, myBox[position], BOX_SETCOLOR, GREY);
			printSuper(cursor(49 + 16 * position, 35), WHITE, GREY, "%s", myMenu[position]);
		}
	}
	return position;
}

int WindowExit() {
	clrs();
	boxID myBox[3];
	char menu[2][6] = { " Y A","TIDAK" };
	myBox[2] = box(DRAW, makebox(cursor(58, 20), 52, 10, LGREY, BOX_FILL));
	printSuper(cursor(72, 22), PNULL, LGREY, "Anda Yakin Ingin Keluar?");
	for (int i = 0; i < 2; i++) {
		myBox[i] = box(DRAW, makebox(cursor(62 + i * 23, 25), 21, 3, (i==0)?RED:GREY, BOX_FILL));
		printSuper(cursor(70+i*23,26), WHITE, (i == 0) ? RED : GREY, menu[i]);
	}
	int pos = 0;
	while (TRUE) {
		int r = _getch();
		if (r == LEFT_ARROW || r == RIGHT_ARROW) {
			box(UPDATE, myBox[pos], BOX_SETCOLOR, GREY);
			printSuper(cursor(70 + pos * 23, 26), WHITE, GREY, menu[pos]);
			pos = (pos == 0) ? 1 : 0;
			box(UPDATE, myBox[pos], BOX_SETCOLOR, RED);
			printSuper(cursor(70 + pos * 23, 26), WHITE, RED, menu[pos]);
		}
		else if (r == ESCAPE) return 0;
		else if (r == BACKSPACE) {
			box(ERASE, myBox[2]);
			return 1;
		}
		else if (r == ENTER) {
			if (pos == 1) box(ERASE, myBox[2]);
			return pos;
		}
	}
}

int WindowCreate() {
	clrs();
	boxID jBox[5];
	jBox[2] = box(DRAW, makebox(cursor(20, 6), 130, 3, LGREY, BOX_FILL));
	printSuper(cursor(22, 7), BLACK, LGREY, "BUAT");
	int ipCount = getIPAddress();
	ipSelect = ipCount;
	char instructA[2][64];
	strcpy(instructA[0], "[+]   Tambahkan Peraturan Menantang Disini!...");
	strcpy(instructA[1], "[-]   Slot Penuh, Hapus beberapa!...");
	printSuper(cursor(20, 11), BLACK, WHITE, "NAMA ANDA\t\t :    %s", hostName);
	printSuper(cursor(20, 13), BLACK, WHITE, "IP ADDRESS\t\t :    %s", ipAddress[ipCount]);
	printSuper(cursor(20, 16), BLACK, WHITE, "PERATURAN KARTU DARE!");
	jBox[4] = box(DRAW, makebox(cursor(20, 17), 129, 20, BLACK, BOX_LINES));
	char menu[2][10] = { "LANJUTKAN","  BATAL" };
	for (int i = 0; i < 2; i++) {
		jBox[i] = box(DRAW, makebox(cursor(106 + 23, 39 + 4 * i), 21, 3, GREY, BOX_FILL));
		printSuper(cursor(112 + 23, 40 + 4 * i), WHITE, GREY, menu[i]);
	}
	jBox[3] = box(DRAW, makebox(cursor(44, 10), 30, 2, BLUE, BOX_LINES));
	for (int a = 0; a < dareCount; a++) {
		setc(cursor(22, 18 + a));
		printf("[%d]   %s",a+1, dareRules[a]);
	}
	printSuper(cursor(22, 18 + dareCount), GREY, WHITE, (dareCount >= 10) ? instructA[1] : instructA[0]);
	int count = 0;
	int color = GREY;
	while (TRUE) {
		if (hostName[0] == NULL) printSuper(cursor(46, 11), color, WHITE, "Ketik Untuk Memberi Nama...");
		int oldCount = count;
		int out = arrowNav(count, 0, 4, NAV_VERTICAL);
		if (out >= 0) count = out;
		else if (((out <= (' ' * -1)) || (out == ' ' * -1) || (out == ENTER * -1)) && count == 0) {
			printSuper(cursor(46, 11), WHITE, WHITE, "Ketik Untuk Memberi Nama...");
			if (out != ENTER * -1) out *= -1;
			else if (out == ENTER * -1) out = 0;
			int wow = scanSuper(cursor(46, 11), &hostName, 27, out);
			if (wow == ENTER || wow == 1) {
				count++;
			}
			else if (wow == ESCAPE || wow == -1) {
				count = 4;
			}
		}
		if (count != 2) {
			jBox[4] = box(UPDATE, jBox[4], BOX_SETCOLOR, BLACK);
		}
		if (count == 1) {
			if (oldCount != count) jBox[3] = box(UPDATE, jBox[3], BOX_SETCURSOR, cursor(44, 12));
			setc(cursor(43, 13));
			printf("%c", 0xAE);
			setc(cursor(75, 13));
			printf("%c", 0xAF);
			int in;
			while (TRUE) {
				in = _getch();
				if (in == LEFT_ARROW) {
					setc(cursor(46, 13));
					for (int i = 0; i < strlen(ipAddress[ipSelect]); i++)printf(" ");
					ipSelect = (ipSelect > 0) ? ipSelect - 1 : 0;
					setc(cursor(46, 13));
					printf("%s", ipAddress[ipSelect]);
				}
				else if (in == RIGHT_ARROW){
					setc(cursor(46, 13));
					for (int i = 0; i < strlen(ipAddress[ipSelect]); i++)printf(" ");
					ipSelect = (ipSelect < ipCount) ? ipSelect + 1 : ipCount;
					setc(cursor(46, 13));
					printf("%s", ipAddress[ipSelect]);
				}
				else if (in == UP_ARROW) {
					jBox[3] = box(UPDATE, jBox[3], BOX_SETCURSOR, cursor(44, 10));
					count = 0;
					break;
				}
				else if (in == DOWN_ARROW) {
					count = 2;
					break;

				}
			}
		}
		if (count != 1) {
			setc(cursor(43, 13));
			printf(" ");
			setc(cursor(75, 13));
			printf(" ");
		}
		if (count == 0) {
			if (oldCount != count) {
				jBox[3] = box(UPDATE, jBox[3], BOX_SETCURSOR, cursor(44, 10));
			}
		}
		else if (count == 2) {
			box(ERASE, jBox[3]);
			jBox[4] = box(UPDATE, jBox[4], BOX_SETCOLOR, BLUE);
			if (out == ENTER * -1) {
				jBox[4] = box(UPDATE, jBox[4], BOX_SETCOLOR, GREEN);
				printSuper(cursor(22, 34), GREY, WHITE, "Tekan + untuk menambah");
				printSuper(cursor(22, 35), GREY, WHITE, "Tekan - untuk menghapus");
				printSuper(cursor(22, 36), GREY, WHITE, "Tekan Enter untuk menyimpan");
				while (TRUE) {
					int r = _getch();
					if (r == ENTER || r == ESCAPE) break;
					else if ((r == '+' || r == '=') && dareCount < 10) {
						printSuper(cursor(22, 18 + dareCount), WHITE, WHITE, instructA[0]);
						setc(cursor(22, 18 + dareCount));
						printf("[%d]", dareCount + 1);
						int out = scanSuper(cursor(28, 18 + dareCount), dareRules[dareCount], 64, 0);
						if (out == ENTER) {
							dareCount++;
						}
						printSuper(cursor(22, 18 + dareCount), GREY, WHITE, (dareCount >= 10) ? instructA[1] : instructA[0]);
					}
					else if ((r == '-' || r == '_') && dareCount > 0) {
						printSuper(cursor(22, 18 + dareCount), WHITE, WHITE, instructA[0]);
						printSuper(cursor(22, 18 + dareCount),RED,WHITE,"[-]   Masukkan Index Untuk Dihapus : ");
						cursorvisibility(TRUE);
						int index;
						scanf("%d", &index);
						cursorvisibility(FALSE);
						box(DRAW, makebox(cursor(22, 18), 64, dareCount + 2, WHITE, BOX_FILL));
						for (int a = index; a < dareCount; a++) {
							strcpy(dareRules[a], dareRules[a + 1]);
						}
						ZeroMemory(dareRules[dareCount],sizeof(dareRules[dareCount]));
						dareCount--;
						for (int a = 0; a < dareCount; a++) {
							setc(cursor(22, 18 + a));
							printf("[%d]   %s", a + 1, dareRules[a]);
						}
						printSuper(cursor(22, 18 + dareCount), GREY, WHITE, instructA[0]);
					}
				}
				jBox[4] = box(UPDATE, jBox[4], BOX_SETCOLOR, BLUE);
				box(DRAW, makebox(cursor(22, 34), 30, 3, WHITE, BOX_FILL));
			}
		}
		else if (count == 3 || count == 4) {
			jBox[count-3] = box(UPDATE, jBox[count-3], BOX_SETCOLOR, BLUE);
			printSuper(cursor(112 + 23, 40 + 4 * (count-3)), WHITE, BLUE, menu[count-3]);
			if (count == 4) jBox[3] = box(ERASE, jBox[3]);
			if (out == ENTER * -1 && count == 3) {
				if (hostName[0] != NULL) {
					strcpy(ipServer, ipAddress[ipSelect]);
					int err = socketInit(AS_SERVER);
					COORD sv = { 20,15 };
					setc(sv);
					if (err == SOCK_NOSERVER) {
						printf("KONEKSI ERROR, MOHON CEK ULANG IP");
					}
					else if (err == SOCK_INITFAIL) {
						printf("SOCKET GAGAL DI INISIASI");
					}
					else if (err == SOCK_ERRADDSK) {
						printf("ERROR ADDRESS SOCKET LISTENING");
					}

					else if (err == SOCK_SERVEROFFLINE) {
						printf("SERVER OFFLINE ATAU TIDAK DITEMUKAN, COBA PERHATIKAN IP SERVER");
					}
					else if (err == SOCK_BINDERROR) {
						printf("BINDING ERROR");
					}
					else if (err == SOCK_SUCCESSINIT) {
						return 1;
					}
					return 1;
				}
				else color = RED;
			}
			else if (out == ENTER * -1 && count == 4) {
				return -1;
			}
		}
		if (count != 3) {
			jBox[0] = box(UPDATE, jBox[0], BOX_SETCOLOR, GREY);
			printSuper(cursor(112 + 23, 40), WHITE, GREY, menu[0]);
		}
		if (count != 4) {
			jBox[1] = box(UPDATE, jBox[1], BOX_SETCOLOR, GREY);
			printSuper(cursor(112 + 23, 44), WHITE, GREY, menu[1]);
		}
	}
}

int WindowJoin() {
	clrs();
	boxID jBox[5];
	jBox[2] = box(DRAW, makebox(cursor(20, 6), 130, 3, LGREY, BOX_FILL));
	printSuper(cursor(22, 7), BLACK, LGREY, "GABUNG");
	printSuper(cursor(20, 11), BLACK, WHITE, "NAMA ANDA\t\t :    %s", hostName);
	printSuper(cursor(20, 13), BLACK, WHITE, "IP SERVER\t\t :    %s", ipServer);
	char menu[2][10] = { "LANJUTKAN","  BATAL" };
	for (int i = 0; i < 2; i++) {
		jBox[i] = box(DRAW, makebox(cursor(106 + 23, 39 + 4 * i), 21, 3, GREY, BOX_FILL));
		printSuper(cursor(112 + 23, 40 + 4 * i), WHITE, GREY, menu[i]);
	}
	jBox[3] = box(DRAW, makebox(cursor(44, 10), 30, 2, BLUE, BOX_LINES));
	int count = 0;
	int color1 = GREY;
	int color2 = GREY;
	while (TRUE) {
		if (hostName[0] == NULL) printSuper(cursor(46, 11), color1, WHITE, "Ketik Untuk Memberi Nama...");
		if (ipServer[0] == NULL) printSuper(cursor(46, 13), color2, WHITE, "Ketik Untuk Mencari Server.");
		int oldCount = count;
		int out = arrowNav(count, 0, 3, NAV_VERTICAL);
		if (out >= 0) count = out;
		else if (((out <= (' ' * -1)) || (out == ' ' * -1) || (out == ENTER * -1)) && count == 0) {
			color1 = GREY;
			printSuper(cursor(46, 11), WHITE, WHITE, "Ketik Untuk Memberi Nama...");
			if (out != ENTER * -1) out *= -1;
			else if (out == ENTER * -1) out = 0;
			int wow = scanSuper(cursor(46, 11), &hostName, 27, out);
			if (wow == ENTER || wow == 1) {
				count++;
			}
			else if (wow == ESCAPE || wow == -1) {
				count = 3;
			}
		}
		else if (((out <= (' ' * -1)) || (out == ' ' * -1) || (out == ENTER * -1)) && count == 1) {
			color2 = GREY;
			printSuper(cursor(46, 13), WHITE, WHITE, "Ketik Untuk Mencari Server.");
			if (out != ENTER * -1) out *= -1;
			else if (out == ENTER * -1) out = 0;
			int wow = scanSuper(cursor(46, 13), &ipServer, 27, out);
			if (wow == ENTER || wow == 1) {
				count++;
			}
			else if (wow == ESCAPE || wow == -1) {
				count--;
			}
		}
		if (count == 0 || count == 1) {
			if (oldCount != count) {
				jBox[3] = box(UPDATE, jBox[3], BOX_SETCURSOR, cursor(44, 10 + 2 * count));
			}
		}
		else if (count == 2 || count == 3) {
			box(ERASE, jBox[3]);
			jBox[count - 2] = box(UPDATE, jBox[count - 2], BOX_SETCOLOR, BLUE);
			printSuper(cursor(112 + 23, 40 + 4 * (count - 2)), WHITE, BLUE, menu[count - 2]);
			if (count == 3) jBox[3] = box(ERASE, jBox[3]);
			if (out == ENTER * -1 && count == 2) {
				if (hostName[0] == NULL || ipServer[0] == NULL) {
					if (hostName[0] == NULL) color1 = RED;
					if (ipServer[0] == NULL) color2 = RED;
				}
				else {
					int err = socketInit(AS_CLIENT);
					COORD sv = { 20,15 };
					setc(sv);
					if (err == SOCK_NOSERVER) {
						printf("KONEKSI ERROR, MOHON PERIKSA KEMBALI IP SERVER YANG DI INPUT");
					}
					else if (err == SOCK_INITFAIL) {
						printf("SOCKET GAGAL DI INISIASI");
					}
					else if (err == SOCK_ERRADDSK) {
						printf("ERROR ADDRESS SOCKET LISTENING");
					}

					else if (err == SOCK_SERVEROFFLINE) {
						printf("SERVER OFFLINE ATAU TIDAK DITEMUKAN, COBA PERHATIKAN IP SERVER");
					}
					else if (err == SOCK_BINDERROR) {
						printf("BINDING ERROR");
					}
					else if (err == SOCK_SUCCESSINIT) {
						return 1;
					}
				}
			}
			else if ((out == ENTER * -1 && count == 3) || out == -27) {
				return -1;
			}
		}
		if (count != 2) {
			jBox[0] = box(UPDATE, jBox[0], BOX_SETCOLOR, GREY);
			printSuper(cursor(112 + 23, 40), WHITE, GREY, menu[0]);
		}
		if (count != 3) {
			jBox[1] = box(UPDATE, jBox[1], BOX_SETCOLOR, GREY);
			printSuper(cursor(112 + 23, 44), WHITE, GREY, menu[1]);
		}
	}
}

int WindowWaiting() {
	int state = (dPass.myID == 0)? AS_SERVER : AS_CLIENT;
	dPass.winState = TRUE;
	clrs();
	COORD c;
	c.X = 84 - (3 * strlen(dPass.myIP));
	c.Y = 11;
	printbig(c, (state == AS_SERVER) ? BLUE : YELLOW, WHITE, dPass.myIP);
	c.Y--;
	box(DRAW, makebox(c, 6 * strlen(dPass.myIP), 6, (state == AS_SERVER) ? BLUE : YELLOW, BOX_LINES));
	printSuper(cursor((state == AS_SERVER) ? 73 : 79,10), BLACK, WHITE, (state == AS_SERVER) ? " IP SERVER UNTUK JOIN " : " IP  SAYA ");
	boxID jBox[5];
	jBox[2] = box(DRAW, makebox(cursor(20, 6), 130, 3, LGREY, BOX_FILL));
	printSuper(cursor(22, 7), BLACK, LGREY, "MENUNGGU PEMAIN");
	box(DRAW, makebox(cursor(27, 17), 114, 2 * 12, BLACK, BOX_LINES));
	printSuper(cursor(59, 42), WHITE, RED,		   "                Untuk Batal, tekan ESC                 ");
	jBox[0] = makebox(cursor(28, 20), 112, 2, BLUE, BOX_LINES);
	if (state == AS_SERVER) {
		printSuper(cursor(59, 43), WHITE, BLUE,   "                Tekan Spasi Untuk Mulai                ");
	}
	int nDump = 11;
	printf("\n");
	while (TRUE) {
		//debug
		int r = getch_nonblock();
		if (r == ESCAPE || dPass.winState == FALSE) {
			user_set(USER_DROP_ALL);
			socketDrop();
			return -1;
		}
		else if (r == SPACE && state == AS_SERVER) {
			return 1;
		}
		else if (dPass.state == PLAY_GAME && state == AS_CLIENT) {
			return 1;
		}
		if (dPass.pCount != nDump || dPass.Update == TRUE) {
			if (nDump < dPass.pCount) { //Nambah Client
				int a = dPass.pCount - 1;
				box(DRAW, makebox(cursor(30, 19 + 2 * a), 110, 1, (pUno[a].ID == dPass.myID) ? GREEN : WHITE, BOX_FILL));
				printSuper(cursor(30, 19 + 2 * a), (pUno[a].ID == dPass.myID) ? WHITE : BLACK, (pUno[a].ID == dPass.myID) ? GREEN : WHITE, (pUno[a].ID == dPass.myID) ? (dPass.myID == 0) ? "[%d]   %s | SERVER" : "[%d]   %s | ME" : "[%d]   %s", a, pUno[a].name);
				printSuper(cursor(70, 19 + 2 * a), (pUno[a].ID == dPass.myID) ? WHITE : BLACK, (pUno[a].ID == dPass.myID) ? GREEN : WHITE, "IP : %s | ID : %d  ", pUno[a].ip,pUno[a].ID);
				printSuper(cursor(108, 19 + 2 * a), (pUno[a].ID == dPass.myID) ? WHITE : BLACK, (pUno[a].ID == dPass.myID) ? GREEN : WHITE, "TIME : % s", asctime(localtime(&pUno[a].signInTime)));
			}
			else if(nDump > dPass.pCount){ //Ngurang Client
				box(DRAW, makebox(cursor(30, 19 + 2 * dPass.pCount), 110, 1, WHITE, BOX_FILL));
				for (int a = 0; a < dPass.pCount; a++) {
					box(DRAW, makebox(cursor(30, 19 + 2 * a), 110, 1, (pUno[a].ID == dPass.myID) ? GREEN : WHITE, BOX_FILL));
					printSuper(cursor(30, 19 + 2 * a), (pUno[a].ID == dPass.myID) ? WHITE : BLACK, (pUno[a].ID == dPass.myID) ? GREEN : WHITE, (pUno[a].ID == dPass.myID) ? (dPass.myID == 0) ? "[%d]   %s | SERVER" : "[%d]   %s | ME" : "[%d]   %s", a, pUno[a].name);
					printSuper(cursor(70, 19 + 2 * a), (pUno[a].ID == dPass.myID) ? WHITE : BLACK, (pUno[a].ID == dPass.myID) ? GREEN : WHITE, "IP : %s | ID : %d  ", pUno[a].ip, pUno[a].ID);
					printSuper(cursor(108, 19 + 2 * a), (pUno[a].ID == dPass.myID) ? WHITE : BLACK, (pUno[a].ID == dPass.myID) ? GREEN : WHITE, "TIME : % s", asctime(localtime(&pUno[a].signInTime)));
				}
			}
			if (dPass.pCount == 10 && state == AS_SERVER) {
				printSuper(cursor(30, 19 + 2 * 10), GREEN, WHITE, "[!]   Room sudah Penuh, silahkan tekan Spasi untuk MULAI");
			}
			else if (state == AS_SERVER) {
				box(DRAW, makebox(cursor(30, 21 + 2 * dPass.pCount), 110, 1, WHITE, BOX_FILL));
				printSuper(cursor(30, 19 + 2 * dPass.pCount), GREY, WHITE, "[+]   Masukkan Alamat IP Server pada PC Client dengan Benar");
			}
			nDump = dPass.pCount;
			dPass.Update = FALSE;
		}
	}
	system("pause");
}

void gameplaygraphics(int loX, int loY) {
	printImageAscii(cursor(66, 16), dPass.Last3[2].cards.dir, ' ', dPass.Last3[2].cards.color, 'H', WHITE, 'A', RED, 'B', YELLOW, 'C', GREEN, 'D', BLUE);
	int color[3] = { RED, YELLOW, GREEN };
	int text[3][2];
	strcpy(text[0], "U");
	strcpy(text[1], "N");
	strcpy(text[2], "O");
	for (int a = 0; a < 3; a++) printbig(cursor(48 + loX + (a * 6), 8 + loY), color[a], PNULL, text[a]);
	box(DRAW, makebox(cursor(4, loY - 1), 38, 22, BLACK, BOX_LINES));
	box(DRAW, makebox(cursor(120, loY - 1), 46, 22, BLACK, BOX_LINES));
	box(DRAW, makebox(cursor(4, 31), 162, 7, BLACK, BOX_LINES));
	box(DRAW, makebox(cursor(4, 39), 162, 7, BLACK, BOX_LINES));
	printSuper(cursor(5, 8), BLACK, WHITE, " PEMAIN ");
	printSuper(cursor(65, 8), BLACK, WHITE, "DROP CARD");
	printSuper(cursor(98, 8), BLACK, WHITE, "DRAW CARD");
	printSuper(cursor(156, 8), BLACK, WHITE, " PETUNJUK ");
	printSuper(cursor(5, 31), BLACK, WHITE, " ANTRIAN KARTU ");
	printSuper(cursor(5, 39), BLACK, WHITE, " KARTU SAYA ");
	printSuper(cursor(123, loY - 8), BLACK, WHITE, "TIMEOUT");
	printbig(cursor(121, 2), LGREY, WHITE, "00");
	printbig(cursor(8, 2), GREEN, WHITE, "20:00");
	printSuper(cursor(18, loY - 8), BLACK, WHITE, "SISA WAKTU");
	printSuper(cursor(154, loY - 8), BLACK, WHITE, "JUMLAH KARTU");
	printSuper(cursor(137, loY - 8), BLACK, WHITE, "JUMLAH PEMAIN");
	setc(cursor(122, 10)); printf("ATAS - untuk mengantrikan kartu");
	setc(cursor(122, 12)); printf("ATAS lagi/ENTER - untuk DROP kartu");
	setc(cursor(122, 14)); printf("BAWAH - untuk menghapus kartu dari antrian");
	setc(cursor(122, 16)); printf("BAWAH lagi/BACKSPACE - untuk DRAW kartu");
	setc(cursor(122, 18)); printf("KANAN/KIRI - untuk navigasi kartu");
	setc(cursor(122, 20)); printf("DELETE - untuk hapus semua kartu antrian");
	setc(cursor(122, 22)); printf("Z - untuk susun kartu sesuai Karakter");
	setc(cursor(122, 24)); printf("X - untuk mengelompokkan warna kartu");
	setc(cursor(122, 26)); printf("SPACE - untuk mengucapkan UNO!");
	setc(cursor(122, 28)); printf("ESCAPE - untuk keluar");
	for (int a = 0; a < dPass.pCount; a++) {
		printSuper(cursor(6, 10 + (a * 2)), BLACK, WHITE, pUno[a].name);
		printSuper(cursor(36, 10 + (a * 2)), BLACK, WHITE, "[%2.d]",pUno[a].cardCount);
	}
}

DWORD WINAPI GamePlayDisp_thread(){
	const int loX = 46;
	const int loY = 9;
	COORD curs[4];
	boxID myBox[4];
	bool update = TRUE;
	for (int a = 0; a < 3; a++) {
		curs[a].Y = loY;
		curs[a].X = loX + (a * 4);
	}
	curs[3].X = 41 + loX;
	curs[3].Y = loY;

	for (int a = 0; a < 4; a++) {
		myBox[a] = box(DRAW, makebox(curs[a], (a < 2)?3:30, 21, (a == 3)?BLACK:dPass.Last3[a].cards.color, BOX_FILL));
		if (a < 3) {
			printSuper(curs[a], WHITE, dPass.Last3[a].cards.color, (a < 2) ? dPass.Last3[a].cards.name_a : dPass.Last3[a].cards.name);
			printSuper(cursor(curs[a].X, loY + 20), WHITE, dPass.Last3[a].cards.color, (a < 2) ? "[%d]" : "[%s]", (a < 2) ? ((dPass.Last3[a].index == -1)? 0 : dPass.Last3[a].index) : ((dPass.Last3[a].index == -1)?"SERVER":pUno[dPass.Last3[a].index].name));
		}
	}
	boxID cardBox[2][40];
	for (int a = 0; a < 40; a++) {
		COORD curs[2];
		curs[0] = cursor(6 + (a * 4), 41);
		curs[1] = cursor(6 + (a * 4), 33);
		cardBox[1][a] = makebox(curs[1], 3, 4, BLACK, BOX_FILL);
		if (a < meOnly.cardCount) {
			cardBox[0][a] = box(UPDATE, makebox(curs[0], 3, 4, meOnly.cards[a].color, BOX_FILL));
			printSuper(curs[0], WHITE, meOnly.cards[a].color, meOnly.cards[a].name_a);
		}
		else cardBox[0][a] = makebox(curs[0], 3, 4, BLACK, BOX_FILL);
	}

	gameplaygraphics(loX,loY);

	//LOCAL VARIAVLE
	boxID boxturn = makebox(cursor(6, 10 + (0 * 2)), 35, 1, LBLUE, BOX_FILL);
	boxID boxSelect = makebox(cursor(5, 40), 4, 5, GREY, BOX_LINES);
	CLKSTR dump = { 0 };
	int timeout = 0;
	int turnDump = -1;
	int playerDump = -1;
	int cardDump = -1;
	int n = 0;
	int qCount = 0;
	int nDump = -1;
	int myTurn = getIndexNumber(dPass.myID);
	int stateDump = 0;
	int dumpIDCards = dPass.Last3[2].cards.ID;
	bool boxState[40] = { 0 };
	bool updatecards = FALSE;
	char firstType;
	while (dPlay.state != GAME_OVER) {
		//DYNAMIC UPDATE
		if (stateDump != dPlay.update) {
			stateDump = dPlay.update;
			if (dPlay.update == UPDATE_PLAYER_CARD_COUNT || dPlay.update == UPDATE_LAST3_CARDS) {
				for (int a = 0; a < dPass.pCount; a++) {
					printSuper(cursor(6, 10 + (a * 2)), (a == myTurn)?WHITE:BLACK, (a == myTurn) ? LBLUE:WHITE, pUno[a].name);
					printSuper(cursor(36, 10 + (a * 2)),(a == myTurn) ? WHITE : BLACK, (a == myTurn) ? LBLUE : WHITE, "[%2.d]", pUno[a].cardCount);
				}

			}
		}
		if (dump.inSec != dPass.elapsed.inSec) { //UPDATE EVERY SECOND
			dump = dPass.elapsed;
			printbig(cursor(8, 2), GREEN, WHITE, dPass.elapsed.clkCh);
			if (timeout > 0 && dPass.state == PLAY_GAME) {
				timeout--;
				char to[3];
				ZeroMemory(to, sizeof(to));
				to[0] = ((timeout - (timeout % 10)) / 10) + '0';
				to[1] = (timeout % 10) + '0';
				printbig(cursor(121, 2), (timeout < 7) ? ((timeout < 3) ? ((timeout == 0) ? LGREY : RED) : YELLOW) : GREEN, WHITE, to);
				if (timeout <= 0) {
					printbig(cursor(57, loY - 7), LGREY, WHITE, "GILIRANMU!");
					PlayGame(PLAYER_TIMEOUT);
				}
			}
		}
		if (turnDump != dPass.turn) {
			if (dPass.turn == myTurn && dPass.state == STATE_IDLE) { //UPDATE MYTURN & TIMEOUT
				dPass.state = PLAY_GAME;
				timeout = TIMEOUT;
				char to[3];
				ZeroMemory(to, sizeof(to));
				to[0] = ((timeout - (timeout % 10)) / 10) + '0';
				to[1] = (timeout % 10) + '0';
				printbig(cursor(121, 2), GREEN, WHITE, to);
			}
			if (dumpIDCards != dPass.Last3[2].cards.ID) {
				dumpIDCards = dPass.Last3[2].cards.ID;
				for (int a = 0; a < 3; a++) {
					myBox[a] = box(UPDATE, myBox[a], BOX_SETCOLOR, dPass.Last3[a].cards.color);
					printSuper(curs[a], WHITE, dPass.Last3[a].cards.color, (a < 2) ? dPass.Last3[a].cards.name_a : dPass.Last3[a].cards.name);
					printSuper(cursor(curs[a].X, loY + 20), WHITE, dPass.Last3[a].cards.color, (a < 2) ? "[%d]" : "[%s]", (a < 2) ? ((dPass.Last3[a].index == -1) ? 0 : dPass.Last3[a].index) : ((dPass.Last3[a].index == -1) ? "SERVER" : pUno[dPass.Last3[a].index].name));
				}
				printImageAscii(cursor(66, 16), dPass.Last3[2].cards.dir, ' ', dPass.Last3[2].cards.color, 'H', WHITE, 'A', RED, 'B', YELLOW, 'C', GREEN, 'D', BLUE);
			}
			printbig(cursor(57, loY - 7), (dPass.turn == myTurn)?RED:LGREY, WHITE, "GILIRANMU!");
			if(dPass.turn != myTurn) printbig(cursor(121, 2), LGREY, WHITE, "00");
			COORD turnCurs = cursor(6, 10 + (dPass.turn * 2));
			boxturn = box(UPDATE, boxturn, BOX_SETCURSOR, turnCurs);
			if (turnDump != -1) {
				printSuper(cursor(6, 10 + (turnDump * 2)), BLACK, WHITE, pUno[turnDump].name);
				printSuper(cursor(36, 10 + (turnDump * 2)), BLACK, WHITE, "[%2.d]", pUno[turnDump].cardCount);
			}
			turnDump = dPass.turn;
			printSuper(turnCurs, WHITE, LBLUE, pUno[dPass.turn].name);
			printSuper(cursor(36, 10 + (turnDump * 2)), WHITE, LBLUE, "[%2.d]", pUno[turnDump].cardCount);
		}
		if (playerDump != dPass.pCount) {
			playerDump = dPass.pCount;
			char to[3] = "00";
			to[0] = ((playerDump - (playerDump % 10)) / 10) + '0';
			to[1] = (playerDump % 10) + '0';
			printbig(cursor(139, 2), (playerDump < 5) ? ((playerDump < 3) ? RED : YELLOW) : GREEN, WHITE, to);
		}
		if (cardDump != meOnly.cardCount) {
			cardDump = meOnly.cardCount;
			char num[3] = "00";
			num[0] = ((meOnly.cardCount - (meOnly.cardCount % 10)) / 10) + '0';
			num[1] = (meOnly.cardCount % 10) + '0';
			printbig(cursor(155, loY - 7), BLUE, WHITE, num);
		}
		if (n != nDump) {
			nDump = n;
			boxSelect = box(UPDATE, boxSelect, BOX_SETCURSOR, cursor(5 + 4 * n, 40), BOX_SETCOLOR, meOnly.cards[n].color);
		}

		//INPUT
		int input = getch_nonblock();
		if (input == LEFT_ARROW) {
			n--;
			if (n < 0) n = meOnly.cardCount - 1;
		}
		else if (input == RIGHT_ARROW) {
			n++;
			if (n >= meOnly.cardCount)n = 0;
		}
		else if (input == 'Z' || input == 'z') {
			cardSort(&boxState, CARD_SORT_BY_CHAR);
			updatecards = TRUE;

		}
		else if (input == 'X' || input == 'x') {
			cardSort(&boxState, CARD_SORT_BY_COLOR);
			updatecards = TRUE;

		}
		else if (input == ESCAPE) {
			d_buf buf;
			buf.state = GAME_OVER;
			if (dPass.myID == AS_SERVER) {
				sendData(SEND_BROADCAST, buf);
			}
			else sendData(pSock.sock, buf);
			dPlay.state = GAME_OVER;
			break;
		}
		else if (input == UP_ARROW || input == ENTER) {
			if (boxState[n] != TRUE && input == UP_ARROW) {
				if (qCount == 0) {
					boxState[n] = TRUE;
					qCount++;
					firstType = meOnly.cards[n].name_a[0];
					cardBox[1][n] = box(UPDATE, cardBox[1][n], BOX_SETCOLOR, meOnly.cards[n].color);
					cardBox[0][n] = box(UPDATE, cardBox[0][n], BOX_SETCOLOR, meOnly.cards[n].color_d);
					printSuper(cursor(6 + (n * 4), 33), WHITE, meOnly.cards[n].color, meOnly.cards[n].name_a);
					printSuper(cursor(6 + (n * 4), 41), BLACK, meOnly.cards[n].color_d, meOnly.cards[n].name_a);
				}
				else if (qCount > 0 && meOnly.cards[n].name_a[0] == firstType) {
					boxState[n] = TRUE;
					cardBox[1][n] = box(UPDATE, cardBox[1][n], BOX_SETCOLOR, meOnly.cards[n].color);
					cardBox[0][n] = box(UPDATE, cardBox[0][n], BOX_SETCOLOR, meOnly.cards[n].color_d);
					printSuper(cursor(6 + (n * 4), 33), WHITE, meOnly.cards[n].color, meOnly.cards[n].name_a);
					printSuper(cursor(6 + (n * 4), 41), BLACK, meOnly.cards[n].color_d, meOnly.cards[n].name_a);
					qCount++;
				}
			}
			else if (boxState[n] == TRUE && dPass.turn == myTurn) {
				int in = 0;
				for (int a = 0; a < meOnly.cardCount; a++) {
					if (boxState[a] == TRUE && meOnly.cards[a].color == BLACK) {
						box(UPDATE, myBox[2], BOX_SETCOLOR, PURPLE);
						printSuper(curs[2], WHITE, PURPLE, meOnly.cards[a].name);
						printImageAscii(cursor(66, 16), meOnly.cards[a].dir, ' ', PURPLE, 'H', WHITE, 'A', RED, 'B', YELLOW, 'C', GREEN, 'D', BLUE);
						printSuper(cursor(58, 25), WHITE, PURPLE, "PILIH WARNA KARTU WILD");
						int bCols[2][4] = { {RED,YELLOW,GREEN,BLUE},{PINK,LYELLOW,LGREEN,LBLUE} };
						boxID bBox[4];
						for (int b = 0; b < 4; b++) {
							bBox[b] = box(DRAW, makebox(cursor(62 + (4 * b), 27), 2, 1, bCols[0][b], BOX_FILL));
						}
						bBox[0] = box(UPDATE, bBox[0], BOX_SETCOLOR, PINK);
						int loc = 0;
						while (1) {
							in = getch_nonblock();
							if (in == LEFT_ARROW && loc > 0) {
								bBox[loc] = box(UPDATE, bBox[loc], BOX_SETCOLOR, bCols[0][loc]);
								loc--;
								bBox[loc] = box(UPDATE, bBox[loc], BOX_SETCOLOR, bCols[1][loc]);
							}
							else if (in == RIGHT_ARROW && loc < 3) {
								bBox[loc] = box(UPDATE, bBox[loc], BOX_SETCOLOR, bCols[0][loc]);
								loc++;
								bBox[loc] = box(UPDATE, bBox[loc], BOX_SETCOLOR, bCols[1][loc]);
							}
							else if (in == ESCAPE) {
								box(DRAW, myBox[2]);
								printSuper(curs[2], WHITE, dPass.Last3[2].cards.color, dPass.Last3[2].cards.name);
								printSuper(cursor(curs[2].X, loY + 20), WHITE, dPass.Last3[2].cards.color, "[%s]", (dPass.Last3[2].index == -1) ? "SERVER" : pUno[dPass.Last3[2].index].name);
								printImageAscii(cursor(66, 16), dPass.Last3[2].cards.dir, ' ', dPass.Last3[2].cards.color, 'H', WHITE, 'A', RED, 'B', YELLOW, 'C', GREEN, 'D', BLUE);
								break;
							}
							else if (in == ENTER) {
								for (int s = 0; s < meOnly.cardCount; s++) {
									if (boxState[s] == TRUE) {
										meOnly.cards[s].color = bCols[0][loc];
										meOnly.cards[s].color_d = bCols[1][loc];
									}
								}
								break;
							}
						}
					}
					else in = ENTER;
				}
				if (in == ENTER) {
					int lastCnt = qCount;
					for (int a = 0; a < meOnly.cardCount; a++) {
						if (boxState[a] == TRUE) {
							box(ERASE, cardBox[1][a]);
							lastCnt--;
						}
						if (lastCnt == 0) break;
					}
					drop(boxState, qCount);
					if (n >= meOnly.cardCount) n = meOnly.cardCount - 1;
					qCount = 0;
					ZeroMemory(&boxState, sizeof(boxState));
				}
			}
		}
		else if (input == DOWN_ARROW || input == BACKSPACE) {
			if (boxState[n] == TRUE && input == DOWN_ARROW) {
				boxState[n] = FALSE;
				cardBox[1][n] = box(ERASE, cardBox[1][n]);
				cardBox[0][n] = box(UPDATE, cardBox[0][n], BOX_SETCOLOR, meOnly.cards[n].color);
				printSuper(cursor(6 + (n * 4), 41), WHITE, meOnly.cards[n].color, meOnly.cards[n].name_a);
				qCount--;
			}
			else if ((boxState[n] != TRUE || input == BACKSPACE) && dPass.turn == myTurn) {
				draw(1);
				timeout = 0;
			}
		}
		//STATIC UPDATE
		if (updatecards == TRUE || meOnly.cardCount != cardDump) {
			if (meOnly.cardCount > 40) break;
			for (int a = 0; a < meOnly.cardCount; a++) {
				if (boxState[a] == TRUE) {
					cardBox[1][a] = box(UPDATE, cardBox[1][a], BOX_SETCOLOR, meOnly.cards[a].color);
					printSuper(cursor(6 + (a * 4), 33), WHITE, meOnly.cards[a].color, meOnly.cards[a].name_a);
				}
				else {
					cardBox[1][a] = box(ERASE, cardBox[1][a]);
				}
				cardBox[0][a] = box(UPDATE, cardBox[0][a], BOX_SETCOLOR, (boxState[a] == TRUE) ? meOnly.cards[a].color_d : meOnly.cards[a].color);
				printSuper(cursor(6 + (a * 4), 41), (boxState[a] == TRUE) ? BLACK : WHITE, (boxState[a] == TRUE) ? meOnly.cards[a].color_d : meOnly.cards[a].color, meOnly.cards[a].name_a);
			}
			if (cardDump > meOnly.cardCount) {
				for (int i = meOnly.cardCount; i < cardDump; i++) {
					box(ERASE, cardBox[0][i]);
				}
			}
			if (cardDump != meOnly.cardCount) {
				char num[3] = "00";
				num[0] = ((meOnly.cardCount - (meOnly.cardCount % 10)) / 10) + '0';
				num[1] = (meOnly.cardCount % 10) + '0';
				printbig(cursor(155, loY - 7), BLUE, WHITE, num);
			}
			boxSelect = box(UPDATE, boxSelect, BOX_SETCOLOR, meOnly.cards[n].color);
			cardDump = meOnly.cardCount;
			if (dPass.pCount != playerDump) {
				playerDump = dPass.pCount;
				char to[2] = { 0 };
				to[0] = ((playerDump - (playerDump % 10)) / 10) + '0';
				to[1] = (playerDump % 10) + '0';
				printbig(cursor(139, 2), (playerDump < 7) ? ((playerDump < 3) ? RED : YELLOW) : GREEN, WHITE, to);
			}
			updatecards = FALSE;
		}
	}
	return 0;
}

int WindowHelp() {
	clrs();
	box(DRAW, makebox(cursor(20, 6), 130, 3, LGREY, BOX_FILL));
	printSuper(cursor(22, 7), BLACK, LGREY, "BANTUAN");
	setc(cursor(0, 10));
	printf("\t\t\t1. Pertama, deck akan di kocok dan tiap pemain akan diberikan 7 kartu.\n\n");
	printf("\t\t\t2. Kedua, untuk memulai game ambil 1 kartu dari deck dan letakkan ditengah meja.\n\n");
	printf("\t\t\t3. Ketiga, mainkan kartu dari tangan anda yang sesuai dengan warna/angka/simbol dengan kartu ditengah meja.\n\n\t\t\t   Anda dapat memainkan lebih dari satu kartu ditangan jika memiliki kartu dengan angka/simbol yang sama.\n\n");
	printf("\t\t\t4. Keempat, jika tidak bisa mengeluarkan kartu dari tangan ambil kartu dari deck.\n\n");
	printf("\t\t\t5. Kelima, perhatikan kartu aksi dan Wild card seperti : \n\n");
	printf("\t\t\t\t A. Kartu Reverse\t  : Membalikkan arah permainan. \n\n");
	printf("\t\t\t\t B. Kartu Skip\t\t  : Melewatkan pemain setelah kartu stop dimainkan. \n\n");
	printf("\t\t\t\t C. Kartu +2\t\t  : Memberi pemain setelah kartu +2 dua kartu dari deck. \n\n");
	printf("\t\t\t\t D. Kartu +4\t\t  : Memberi pemain setelah kartu 4 empat kartu dari deck \n\n\t\t\t\t\t\t\t    dan yang mengeluarkan memilih warna yang ingin dimainkan. \n\n");
	printf("\t\t\t\t E. Kartu Mengganti Warna : Mengganti warna kartu yang akan dimainkan \n\n\t\t\t\t\t\t\t    sesuai pilihan yang memainkan kartu. \n\n");
	printf("\t\t\t6. Keenam, jika kartu anda tersisa 1 ucapkan UNO!, jika orang lain yang mengucapkannya ambil 2 kartu dari deck. \n\n");
	printf("\t\t\t7. Ketujuh, mainkan kartu terakhir anda untuk memenangkan gamenya.");
	setc(cursor(22, 42)); printf("TEKAN SEMBARANG UNTUK KEMBALI");
	return _getch();

}

int WindowAbout() {
	clrs();
	box(DRAW, makebox(cursor(20, 6), 130, 3, LGREY, BOX_FILL));
	printSuper(cursor(22, 7), BLACK, LGREY, "TENTANG");
	setc(cursor(0, 10));
	printf("\t\t\tUNO merupakan permainan kartu yang biasa dimainkan dari 2-10 orang. Tujuan dari permainan UNO adalah\n\n\t\t\tmenghabiskan kartu di tangan secepatnya. Ada 120 kartu UNO : \n\n");
	printf("\t\t\t\tA. Empat Kartu WILD, Empat Kartu WILD Draw Four, dan Empat Kartu Dare \n\n");
	printf("\t\t\t\tB. Masing-masing 25 kartu berwarna MERAH,KUNING,HIJAU,BIRU \n\n\n");
	printf("\t\t\tMasing-masing warna memliki satu angka 0, dua angka 1-9, dan dua simbol SWAP,BLOCK,DRAW TWO,REVERSE. Tiga tipe\n\n\t\t\tkartu terakhir adalah kartu aksi. ");
	setc(cursor(22, 30)); printf("TEKAN SEMBARANG UNTUK KEMBALI");
	return _getch();
}

#endif // ! _DISPLAYCONSOLE_H
