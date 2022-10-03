#pragma once		//헤더 중복 방지
#include <Windows.h>

void gotoxy(int x, int y);

void setConsoleSize(int x, int y);

BOOL IsKeyDown(int key);