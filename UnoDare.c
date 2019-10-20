#include "Core.h"

int main(){
	printf("TEKAN APAPUN untuk FULLSCREEN\nSPASI untuk WINDOWED");
	if (_getch() == SPACE) win = 1;
	initDisplay();
	Sleep(10);
	srand(time(0));
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
	setRes(0, 0);
	return 0;
}