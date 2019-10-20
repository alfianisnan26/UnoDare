#ifndef _SOCKETH_H
#define _SOCKETH_H
#define BUFFERING Sleep(10)

#define UPDATE_TURN 0x3938
#define DRAW_CARDS 0x3937
#define CARDS_PACKAGE 0x3936
#define UPDATE_PLAYER_CARD_COUNT 0x3935

void broadcast(fd_set master, char* buf, int len) {
	for (int i = 0; i != (int)master.fd_count; i++) {
		SOCKET outSock = master.fd_array[i];
		send(outSock, buf, len, 0);
	}
	return;
}

void sendData(int toID, d_buf buf) {
	char cBuf[SZE];
	memcpy(cBuf, &buf, SZE);
	if (toID == SEND_BROADCAST) {
		fd_set master = *dPlay.master;
		for (int i = 0; i != (int)master.fd_count; i++) {
			SOCKET outSock = master.fd_array[i];
			send(outSock, cBuf, SZE, 0);
		}
	}
	else {
		send((SOCKET)toID, cBuf, SZE, 0);
	}
	return;
}

d_buf getData() {
	char cBuf[SZE] = { 0 };
	d_buf buf = { 0 };
	recv(pSock.sock, cBuf, SZE, 1);
	memcpy(&buf, cBuf, SZE);
	return buf;
}

void PlayGame(int state) {
	if (state == PLAYER_TIMEOUT || state == PLAYER_NEXT) {
		d_buf buf;
		if (state == PLAYER_TIMEOUT) meOnly.lives--;
		if (dPass.myID == 0) {
			dPass.turn += dPass.inverse;
			if (dPass.turn >= dPass.pCount) dPass.turn = 0;
			else if (dPass.turn < 0)dPass.turn = dPass.pCount - 1;
			buf.state = UPDATE_TURN;
			buf.turn = dPass.turn;
			dPass.state = STATE_IDLE;
		}
		else {
			buf.state = state;
			buf.turn = dPass.turn;
		}
		sendData((dPass.myID == 0) ? SEND_BROADCAST : (SOCKET)pSock.sock, buf);
	}
}

void updateUserData() {
	d_buf buf;
	buf.state = UPDATE_DATA;
	for (int a = 0; a < dPass.pCount; a++) {
		buf.data = a;
		buf.pUno = pUno[a];
		sendData(SEND_BROADCAST, buf);
	}
	buf.state = UPDATE_PCOUNT;
	buf.data = dPass.pCount;
	sendData(SEND_BROADCAST, buf);
}

void socketDrop() {
	TerminateThread(pSock.rxThread, 0);
	TerminateThread(pSock.txThread, 0);
	WSACleanup();
}

DWORD WINAPI ServerThread(void) {
	fd_set master;
	FD_ZERO(&master);
	FD_SET(pSock.sock, &master);
	while (1) {
		fd_set copy = master;
		dPlay.master = &master;
		int socketCount = select(0, &copy, NULL, NULL, 0);
		for (int i = 0; i < socketCount; i++) {
			SOCKET sock = copy.fd_array[i];
			if (sock == pSock.sock && dPlay.state != PLAY_GAME) {
				//Accept a new Connection
				struct sockaddr_in recs = { 0 };
				socklen_t socklen = sizeof(recs);
				IPID ips;

				SOCKET client = accept(pSock.sock, (struct sockaddr*) & recs, &socklen);

				//Add the new connection to the list of connected client
				FD_SET(client, &master);

				strcpy(ips.ClientIP, inet_ntoa(recs.sin_addr));
				strcpy(ips.ServerIP, ipServer);
				strcpy(ips.ServerName, hostName);

				ips.ClientID = (int)client;
				char sbuf[sizeof(ips)];
				memcpy(sbuf, &ips, sizeof(ips));
				send(client, sbuf, sizeof(ips), 0);
				ZeroMemory(sbuf, sizeof(sbuf));
				recv(client, sbuf, sizeof(ips), 0);
				memcpy(&ips, sbuf, sizeof(ips));
				user_set(USER_ADD, makeuser(ips.ClientIP, ips.ClientName, ips.ClientID));
				updateUserData();
				
			}
			else {
				d_buf buf = { 0 };
				char cbuf[SZE] = { 0 };
				//Received
				int bytesIn = recv(sock, cbuf, SZE, 0);
				if (bytesIn <= 0) {
					//DropClient
					user_set(USER_DROP_BY_ID, (int)sock);
					closesocket(sock);
					FD_CLR(sock, &master);
					updateUserData();
				}
				else if (cbuf[0] != 13) {
					memcpy(&buf, cbuf, SZE);
					if (buf.state == PLAYER_TIMEOUT || buf.state == PLAYER_NEXT) {
						dPass.turn += dPass.inverse;
						if (dPass.turn >= dPass.pCount) dPass.turn = 0;
						else if (dPass.turn < 0)dPass.turn = dPass.pCount - 1;
						buf.state = UPDATE_TURN;
						buf.turn = dPass.turn;
						dPass.state = STATE_IDLE;
						sendData(SEND_BROADCAST, buf);
					}
					else if (buf.state == DRAW_CARDS) {
						buf.state = CARDS_PACKAGE;
						for (int a = 0; a < buf.data; a++) {
							dPass.cardLeft--;
							buf.pUno.cards[a] = dPass.allCards[dPass.cardLeft - a];
							ZeroMemory(&dPass.allCards[dPass.cardLeft - a], sizeof(dPass.allCards[dPass.cardLeft - a]));
						}
						sendData(sock, buf);
						buf.state = UPDATE_PLAYER_CARD_COUNT;
						pUno[getIndexNumber(sock)].cardCount += buf.data;
						buf.data = getIndexNumber(sock);
						buf.pUno.cardCount = pUno[buf.data].cardCount;
						sendData(SEND_BROADCAST, buf);
						PlayGame(PLAYER_NEXT);
					}
					else if (buf.state == PLAYER_DROP_CARDS) {
						for (int a = 0; a < buf.turn; a++) {
							dPass.allCards[dPass.cardLeft] = buf.pUno.cards[a];
							dPass.cardLeft++;
							if (buf.pUno.cards[a].name_a[0] == 'B') {
								dPass.turn += dPass.inverse;
								if (dPass.turn < 0) dPass.turn = dPass.pCount - 1;
								else if (dPass.turn >= dPass.pCount) dPass.turn = 0;
							}
						}
						buf.turn = dPass.turn;
						pUno[buf.data].cardCount = buf.pUno.cardCount;
						for (int a = 0; a < 3; a++) dPass.Last3[a] = buf.Last3[a];
						for (int a = 0; a < 240; a++) {
							int d = randNumRange(0, 60);
							int x = randNumRange(60, 120);
							CARDS dump = dPass.allCards[d];
							dPass.allCards[d] = dPass.allCards[x];
							dPass.allCards[x] = dump;
						}
						sendData(SEND_BROADCAST, buf);
						PlayGame(PLAYER_NEXT);
					}
					else if (buf.state == GAME_OVER) {
						sendData(SEND_BROADCAST, buf);
						dPlay.state = buf.state;
					}
				}
			}
		}
	}
	return 0;
}

DWORD WINAPI ClientThread() {
	while (1) {
		d_buf buf;
		char cBuf[SZE];
		recv(pSock.sock, cBuf, SZE, 0);
		memcpy(&buf, cBuf, SZE);
		if (buf.state == PLAY_GAME) {
			meOnly = pUno[getIndexNumber(dPass.myID)];
			dPass.pCount = buf.data;
			dPass.turn = buf.turn;
			for (int a = 0; a < 3; a++)dPass.Last3[a] = buf.Last3[a];
			dPass.state = PLAY_GAME;
		}
		else if (buf.state == UPDATE_PCOUNT) {
			dPass.pCount = buf.data;
		}
		else if (buf.state == UPDATE_DATA) {
			pUno[buf.data] = buf.pUno;
		}
		else if (buf.state == UPDATE_TURN) {
			dPass.turn = buf.turn;
			dPass.state = STATE_IDLE;
		}
		else if (buf.state == CARDS_PACKAGE) {
			for (int a = 0; a < buf.data; a++) {
				meOnly.cards[meOnly.cardCount + a] = buf.pUno.cards[a];
			}
			meOnly.cardCount += buf.data;
		}
		else if (buf.state == UPDATE_PLAYER_CARD_COUNT) {
			pUno[buf.data].cardCount = buf.pUno.cardCount;
		}
		else if (buf.state == PLAYER_DROP_CARDS) {
			dPass.turn = buf.turn;
			pUno[buf.data].cardCount = buf.pUno.cardCount;
			for (int a = 0; a < 3; a++)dPass.Last3[a] = buf.Last3[a];
			dPass.state = STATE_IDLE;
		}
		else if (buf.state == GAME_OVER) {
			dPlay.state = GAME_OVER;
		}
	}
}

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
		//user_set(USER_ADD, makeuser(ipServer, servName, 0));
		//user_set(USER_ADD, makeuser(dPass.myIP, hostName, dPass.myID));
		pSock.rxThread = CreateThread(NULL, 0, ClientThread, NULL, 0, NULL);
	}
	return SOCK_SUCCESSINIT;
}

#endif