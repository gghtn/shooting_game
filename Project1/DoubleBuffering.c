#include "DoubleBuffering.h"
#include <string.h>
#include <Windows.h>
#include <conio.h>
static int g_nScreenIndex;
static HANDLE g_hScreen[2];

extern int x, y;
//ȭ�� ���� �ʱ�ȭ
void ScreenInit()
{
    CONSOLE_CURSOR_INFO cci;

    // ȭ�� ���� 2���� �����.
    g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    // Ŀ���� �����.
    cci.dwSize = 1;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(g_hScreen[0], &cci);
    SetConsoleCursorInfo(g_hScreen[1], &cci);
}

//Ȱ��ȭ�� ���ۿ� ��Ȱ��ȭ ���� ��ü
void ScreenFlipping()
{
    SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
    g_nScreenIndex = !g_nScreenIndex;
}

//���۰� ȭ�鿡 ǥ�õǴ� ���� ��Ȱ��ȭ�� ���۸� ����� �Լ�
void ScreenClear()
{
    //��ǥ
    COORD Coor = { 0, 0 };
    //cpu�� �� ���� ó���� �� �ִ� �������� ũ�� ���� WORD
    //DWORD(unsigned long), bool(char), BOOL(int), BYTE(unsigned char), WORD(unsigned short), UINT(unsigned int)
    DWORD dw;
    //(���ϴ� ��, ���ϴ� ����(' '), ���ϴ� ����, ���ϴ� Ŀ��, ���̸� ��ȯ�� ��)
    FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', 100 * 50, Coor, &dw);
}

//(x, y)��ǥ�� string ���
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