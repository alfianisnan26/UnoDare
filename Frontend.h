#ifndef _FRONTEND_H
#define _FRONTEND_H

/*##################################################################################################################*/
//LOW LEVEL FRONTEND

struct All_Handle {
	HANDLE stdInput;
	HANDLE stdOutput;
	HANDLE stdError;
} Handle;

void get_handle() {
	Handle.stdInput = GetStdHandle(STD_INPUT_HANDLE);
	Handle.stdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	Handle.stdError = GetStdHandle(STD_ERROR_HANDLE);
}

#define FULLSCREEN 1
#define WINDOWED 2
void windowMode(int state) {
	SetConsoleDisplayMode(Handle.stdOutput, (state == FULLSCREEN) ? TRUE : FALSE, 0);
}

void cursorvisibility(int state) {
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(Handle.stdOutput, &info);
	info.dwSize = 1;
	info.bVisible = state;
	SetConsoleCursorInfo(Handle.stdOutput, &info);
}

void setsystemcolor(int fg, int bg) {
	char mycolor[9] = "color ab";
	char chfg, chbg;
	if (fg >= 0 && fg < 10)chfg = fg + '0';
	else if (fg >= 10 && fg < 16)chfg = (fg - 10) + 'A';
	if (bg >= 0 && bg < 10)chbg = bg + '0';
	else if (bg >= 10 && bg < 16)chbg = (bg - 10) + 'A';
	mycolor[6] = chbg;
	mycolor[7] = chfg;
	system(mycolor);
}

void windowresizelock() {
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

void setscreenbuffersize(int x, int y) {
	char mode[128] = "mode con cols=";//100 lines=32");
	char modelines[8] = " lines=";
	char numx[4], numy[4];
	sprintf(numx, "%d", x);
	sprintf(numy, "%d", y);
	strcat(&mode, numx);
	strcat(&mode, modelines);
	strcat(&mode, numy);
	system(mode);
}

COORD cursor(x, y) {
	COORD myCursor;
	myCursor.X = x;
	myCursor.Y = y;
	return myCursor;
}

COORD getconsolecursorpos() {
	COORD cursor;
	CONSOLE_SCREEN_BUFFER_INFO psBI;
	GetConsoleScreenBufferInfo(Handle.stdOutput, &psBI);
	return psBI.dwCursorPosition;
}

#define COLORFORG 1
#define COLORBACK 2
int getconsolecolor(int colortype) {
	CONSOLE_SCREEN_BUFFER_INFO psBI;
	GetConsoleScreenBufferInfo(Handle.stdOutput, &psBI);
	if (colortype == COLORFORG) {
		WORD dump = psBI.wAttributes >> 4 << 4;
		dump = psBI.wAttributes - dump;
		return (int)dump;
	}
	else if (colortype == COLORBACK) {
		return (int)psBI.wAttributes >> 4;
	}
}

#define GETCENTER 0xFFFF
COORD getCenter(int X, int Y) {
	CONSOLE_SCREEN_BUFFER_INFO psBI;
	GetConsoleScreenBufferInfo(Handle.stdOutput, &psBI);
	if (X == GETCENTER) X = (int)psBI.dwMaximumWindowSize.X / 2;
	if(Y == GETCENTER) Y = (int)psBI.dwMaximumWindowSize.Y / 2;
	cursor(X, Y);
}

#define CENTER_OF_SCREEN 0xFFFF
#define CURSOR_FROM_CENTER 0xFFF0
#define CONTINUE_Y 0xFFF1
#define CONTINUE_X 0xFFF2
COORD setc(COORD cursor) {
	CONSOLE_SCREEN_BUFFER_INFO psBI;
	GetConsoleScreenBufferInfo(Handle.stdOutput, &psBI);
	if (cursor.X == CONTINUE_X) cursor.X = psBI.dwCursorPosition.X;
	if (cursor.Y == CONTINUE_Y) cursor.Y = psBI.dwCursorPosition.Y;
	COORD pos;
	pos.X = cursor.X; pos.Y = cursor.Y;
	SetConsoleCursorPosition(Handle.stdOutput, pos);
	return pos;
}

//GRAPHICAL FRONTEND
#define PNULL -1
void printSuper(COORD cursor, int ForgC, int BackC, const char* val, ...) {
	CONSOLE_SCREEN_BUFFER_INFO psBI;
	GetConsoleScreenBufferInfo(Handle.stdOutput, &psBI);
	if (cursor.X == -1) {
		cursor.X = psBI.dwCursorPosition.X;
	}
	if (cursor.Y == -1) {
		cursor.Y = psBI.dwCursorPosition.Y;
	}
	setc(cursor);
	va_list arg_list;
	int my_arg;
	va_start(arg_list, val);
	int charlen = strlen(val);
	WORD wOldColorAttrs;
	wOldColorAttrs = psBI.wAttributes;
	if (ForgC == PNULL && BackC != PNULL) {
		WORD dump = wOldColorAttrs >> 4 << 4;
		dump = wOldColorAttrs - dump;
		WORD wColor = ((BackC & 0x0F) << 4 + dump);
		SetConsoleTextAttribute(Handle.stdOutput, wColor);
	}
	else if (BackC == PNULL && ForgC != PNULL) {
		WORD wColor = ((wOldColorAttrs >> 4) << 4 + (ForgC & 0x0F));
		SetConsoleTextAttribute(Handle.stdOutput, wColor);
	}
	else if (ForgC != PNULL && BackC != PNULL) {
		WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
		SetConsoleTextAttribute(Handle.stdOutput, wColor);
	}
	for (my_arg = 0; my_arg < charlen; my_arg++) {
		if (val[my_arg] == '%') {
			int r = 0;
			char value[16];
			while (my_arg < charlen) {
				value[r] = val[my_arg];
				value[r + 1] = 0;
				if (val[my_arg] == 'd' || val[my_arg] == 'c' || val[my_arg] == 's') {
					printf(value, va_arg(arg_list, int));
					break;
				}
				else if (val[my_arg] == 'f') {
					printf(value, va_arg(arg_list, double));
					break;
				}
				my_arg++; r++;
			}
		}
		else if (val[my_arg] != '%')printf("%c", val[my_arg]);
	}
	va_end(arg_list);
	SetConsoleTextAttribute(Handle.stdOutput, wOldColorAttrs);
}

void printImageAscii(COORD cursor_val, const char* filename, char x, int color, ...) {
	if (cursor_val.X == -1 && cursor_val.Y == -1) {
		cursor_val = getconsolecursorpos();
	}
	va_list xcols;
	va_start(xcols, color);
	char text[20];
	int colr[20];
	ZeroMemory(text, sizeof(text));
	ZeroMemory(colr, sizeof(colr));
	text[0] = x;
	colr[0] = color;
	int count = 0;
	for (int a = 1; a <= 20; a++) {
		text[a] = va_arg(xcols, char);
		if (text[a] == NULL) break;
		colr[a] = va_arg(xcols, int);
		count++;
	}
	FILE* fp = fopen(filename, "r");
	if (fp <= 0) {
		setc(cursor_val); printf("FILE %s NOT FOUND", filename);
		return;
	}
	int myX = cursor_val.X;
	while (1) {
		char x = fgetc(fp);
		if (x == EOF)break;
		for (int a = 0; a < count; a++) {
			if(x == text[a]) printSuper(cursor_val, colr[a], colr[a], " ");
		}
		if (x == '\n') {
			cursor_val.Y++;
			cursor_val.X = myX-1;
		}
		cursor_val.X++;
	}
	va_end(xcols);
	fclose(fp);
	return;
}

typedef struct boxID_struct {
	COORD curs;
	int sizeX;
	int sizeY;
	int type;
	int color;
} boxID;

#define BOX_LINES 1
#define BOX_FILL 2
boxID makebox(COORD curs, int sizeX, int sizeY, int color, int type) {
	boxID myBox;
	myBox.curs = curs;
	myBox.sizeX = sizeX;
	myBox.sizeY = sizeY;
	myBox.color = color;
	myBox.type = type;
	return myBox;
}

#define DRAW 1
#define ERASE 0
#define UPDATE 2
boxID loopForBox(int state, boxID box_ID) {
	if (box_ID.type == BOX_LINES) {
		int backg = getconsolecolor(COLORBACK);
		int forg;
		if (state == DRAW) {
			forg = box_ID.color;
		}
		else if (state == ERASE) {
			forg = backg;
		}
		printSuper(box_ID.curs, forg, backg, "%c", 218);
		for (int a = box_ID.curs.X; a < box_ID.curs.X + box_ID.sizeX; a++) printSuper(cursor(PNULL, PNULL), forg, backg, "%c", 196);
		printSuper(cursor(box_ID.curs.X + box_ID.sizeX, box_ID.curs.Y), forg, backg, "%c", 191);
		for (int a = box_ID.curs.Y + 1; a < box_ID.curs.Y + box_ID.sizeY; a++) {
			printSuper(cursor(box_ID.curs.X, a), forg, backg, "%c", 179);
			printSuper(cursor(box_ID.curs.X + box_ID.sizeX, a), forg, backg, "%c", 179);
		}
		printSuper(cursor(box_ID.curs.X, box_ID.curs.Y + box_ID.sizeY), forg, backg, "%c", 192);
		for (int a = box_ID.curs.X; a < box_ID.curs.X + box_ID.sizeX; a++) printSuper(cursor(PNULL, PNULL), forg, backg, "%c", 196);
		printSuper(cursor(box_ID.curs.X + box_ID.sizeX, box_ID.curs.Y + box_ID.sizeY), forg, backg, "%c", 217);
	}
	else if (box_ID.type == BOX_FILL) {
		int backg;
		if (state == DRAW) backg = box_ID.color;
		else backg = getconsolecolor(COLORBACK);
		for (int i = 0; i < box_ID.sizeY; i++) {
			for (int j = 0; j < box_ID.sizeX; j++) {
				printSuper(cursor((box_ID.curs.X + j), (box_ID.curs.Y + i)), backg, backg, " ");
			}
		}
	}
	return box_ID;
}

#define BOX_SETCOLOR 1
#define BOX_SETSIZEX 2
#define BOX_SETSIZEY 3
#define BOX_SETCURSOR 4
#define BOX_SETTYPE 5
boxID box(int state, boxID box_ID,...) {
	if (state == UPDATE) {
		int eraseState = FALSE;
		boxID box_ID_Old = box_ID;
		va_list xcols;
		va_start(xcols, box_ID);
		for (int a = 0; a < 5; a++) {
			int Ttype = va_arg(xcols, int);
			if (Ttype < 1 || Ttype > 5) break;
			else if (Ttype == BOX_SETCOLOR) {
				box_ID.color = va_arg(xcols, int);
				if (eraseState != TRUE) eraseState = FALSE;
			}
			else if (Ttype == BOX_SETCURSOR) {
				box_ID.curs = va_arg(xcols, COORD);
				eraseState = TRUE;
			}
			else if (Ttype == BOX_SETSIZEX) {
				box_ID.sizeX = va_arg(xcols, int);
				if ((box_ID.sizeX < box_ID_Old.sizeX) && eraseState == FALSE) {
					eraseState = TRUE;
				}
			}
			else if (Ttype == BOX_SETSIZEY) {
				box_ID.sizeY = va_arg(xcols, int);
				if ((box_ID.sizeY < box_ID_Old.sizeY) && eraseState == FALSE) {
					eraseState = TRUE;
				}
			}
			else if (Ttype == BOX_SETTYPE) {
				box_ID.type = va_arg(xcols, int);
				if ((box_ID.type < box_ID_Old.type) && eraseState == FALSE) {
					eraseState = TRUE;
				}
			}
		}
		va_end(xcols);
		if(eraseState != FALSE) loopForBox(ERASE, box_ID_Old);
		loopForBox(DRAW, box_ID);
	}
	else {
		loopForBox(state, box_ID);
	}
	return box_ID;
}

void clrs() {
	system("cls");
}

#include <stdbool.h>
int initial = FALSE;
bool ascii[4096];

void printbig(COORD curs, int ForgC, int BackC, char* strg) {
	if (initial == FALSE) {
		initial = TRUE;
		FILE* fp = fopen("Data\\char.txt", "r");
		if (fp <= 0) {
			setc(curs); printf("FILE \"Data\\char.txt\" NOT FOUND, PLEASE REINSTALL!");
			return;
		}
		int count = 0;
		ZeroMemory(ascii, sizeof(ascii));
		while (TRUE) {
			char r = fgetc(fp);
			if (r == EOF) break;
			else if (r == ' ') {
				ascii[count] = FALSE;
				count++;
			}
			else if (r == 'H') {
				ascii[count] = TRUE;
				count++;
			}
		}
		fclose(fp);
	}
	COORD dump = curs;
	for (int n = 0; n < strlen(strg); n++) {
		for (int a = 0; a < 5; a++) {
			for (int b = 0; b < 5; b++) {
				if (BackC == PNULL && strg[n] != ' ' && (ascii[(b + 5 * a) + (25 * (strg[n] - 32))] == FALSE));
				else printSuper(curs, BLACK, (ascii[(b + 5 * a) + (25 * (strg[n] - 32))] == TRUE) ? ForgC : BackC, " ");
				curs.X++;
			}
			curs.Y++;
			curs.X = dump.X;
		}
		dump.X += 6;
		curs = dump;
	}
}

CLKSTR updateCLK(CLKSTR clk) {
	clk.min = (clk.inSec - (clk.inSec % 60)) / 60;
	clk.sec = (clk.inSec % 60);
	clk.minCh[0] = ((clk.min - (clk.min % 10)) / 10) + '0';
	clk.minCh[1] = (clk.min % 10) + '0';
	clk.secCh[0] = ((clk.sec - (clk.sec % 10)) / 10) + '0';
	clk.secCh[1] = (clk.sec % 10) + '0';
	clk.clkCh[0] = clk.minCh[0];
	clk.clkCh[1] = clk.minCh[1];
	clk.clkCh[2] = ':';
	clk.clkCh[3] = clk.secCh[0];
	clk.clkCh[4] = clk.secCh[1];

	return clk;
}

#endif