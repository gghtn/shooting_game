#pragma once		//��� �ߺ� ����
#include <Windows.h>

// x,y ��ǥ�� �̵�
void gotoxy(int x, int y);

//�ܼ�â ũ�� ����
void setConsoleSize(int x, int y);

//Ű�ٿ�
BOOL IsKeyDown(int key);

//Ŀ������
void CursorView();