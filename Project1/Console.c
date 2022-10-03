#include "Console.h"
#include <stdio.h>
#pragma warning(disable:4996)

// x,y 좌표로 이동
void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//콘솔창 크기 조절
void setConsoleSize(int x, int y) {
	char command[100];
	sprintf(command, "mode con lines=%d cols=%d", y, x);
	system(command);
}

//키다운
BOOL IsKeyDown(int key) {
	return ((GetAsyncKeyState(key) & 0x8000) != 0);
}

//커서설정
void CursorView() {
	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 1;			//커서 굵기
	cursor.bVisible = FALSE;	//커서 visible
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}