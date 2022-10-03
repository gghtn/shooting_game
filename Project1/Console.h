#pragma once		//헤더 중복 방지
#include <Windows.h>

// x,y 좌표로 이동
void gotoxy(int x, int y);

//콘솔창 크기 조절
void setConsoleSize(int x, int y);

//키다운
BOOL IsKeyDown(int key);

//커서설정
void CursorView();