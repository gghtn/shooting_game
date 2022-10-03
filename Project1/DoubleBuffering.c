#include "DoubleBuffering.h"
#include <string.h>
#include <Windows.h>
#include <conio.h>
static int g_nScreenIndex;
static HANDLE g_hScreen[2];

extern int x, y;
//화면 버퍼 초기화
void ScreenInit()
{
    CONSOLE_CURSOR_INFO cci;

    // 화면 버퍼 2개를 만든다.
    g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    // 커서를 숨긴다.
    cci.dwSize = 1;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(g_hScreen[0], &cci);
    SetConsoleCursorInfo(g_hScreen[1], &cci);
}

//활성화된 버퍼와 비활성화 버퍼 교체
void ScreenFlipping()
{
    SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
    g_nScreenIndex = !g_nScreenIndex;
}

//버퍼가 화면에 표시되는 동안 비활성화된 버퍼를 지우는 함수
void ScreenClear()
{
    //좌표
    COORD Coor = { 0, 0 };
    //cpu가 한 번에 처리할 수 있는 데이터의 크기 단위 WORD
    //DWORD(unsigned long), bool(char), BOOL(int), BYTE(unsigned char), WORD(unsigned short), UINT(unsigned int)
    DWORD dw;
    //(원하는 곳, 원하는 문자(' '), 원하는 넓이, 원하는 커서, 넓이를 반환할 곳)
    FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', 100 * 50, Coor, &dw);
}

//(x, y)좌표에 string 출력
void ScreenPrint(int x, int y, char* string)
{
    DWORD dw;
    COORD CursorPosition = { x, y };
    SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
    WriteFile(g_hScreen[g_nScreenIndex], string, strlen(string), &dw, NULL);
}

void ScreenRelease()
{
    CloseHandle(g_hScreen[0]);
    CloseHandle(g_hScreen[1]);
}