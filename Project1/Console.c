#include "Console.h"
#include <stdio.h>
#pragma warning(disable:4996)

// x,y ��ǥ�� �̵�
void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//�ܼ�â ũ�� ����
void setConsoleSize(int x, int y) {
	char command[100];
	sprintf(command, "mode con lines=%d cols=%d", y, x);
	system(command);
}