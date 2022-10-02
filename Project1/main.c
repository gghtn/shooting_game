#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "Console.h"
#include "DoubleBuffering.h"
#pragma warning(disable:4996)

#define TRUE 1
#define FALSE 0
#define BULLET_SPEED 50
#define BULLET_COUNT 10

enum { UP = 72, LEFT = 75, RIGHT = 77, DOWN = 80, ENTER = 13, ESC = 27, SPACE = 32 };
int x = 32, y = 45;

typedef int BOOL;

typedef struct {
    BOOL bullet_exist;  //총알의 존재여부
    BOOL bullet_shot;   //발사 여부
    int bx, by;         //총알의 좌표
    int b_speed;        //속도 결정을 위한 변수
}bullet;

//버퍼에 출력하는 함수
void Render(int key, char(*string)[7]);
void RenderBullet(bullet* bull);

int main() {

	setConsoleSize(70, 50);
	ScreenInit();
    
    gotoxy(x, y);
	printf("  △  ");
    gotoxy(x, y+1);
	printf("◁■▷");

    char str[2][7] = { "  △  ", "◁■▷" };
    bullet* bull = (bullet*)malloc(sizeof(bullet)* BULLET_COUNT);
    if (bull == NULL) return 0;
    for (int i = 0; i < BULLET_COUNT; i++) {    //모든 총알 초기화
        bull[i].bullet_exist = FALSE;
        bull[i].bullet_shot = FALSE;
    }
    

    while (1) {
        if (kbhit()) {
            int key = getch();
            if (key == ESC) {
                ScreenRelease();
                system("cls");
                printf("프로그램 종료");
                break;
            }
            else if (key == SPACE) { 
                for (int i = 0; i < BULLET_COUNT && !bull[i].bullet_exist; i++) {   //i번째 총알이 존재하지 않으면 발사
                    bull[i].bullet_exist = TRUE;
                    bull[i].bullet_shot = TRUE;
                    bull[i].b_speed = 0;
                    break;
                }
            }
            Render(key, str);
        }
        else {
            Render(-1, str);
        }

        RenderBullet(bull);

        //버퍼교체
        ScreenFlipping();
    }
	return 0;
}

//총알 출력
void RenderBullet(bullet* bull) {
    for (int i = 0; i < BULLET_COUNT; i++) {
        if (bull[i].bullet_shot) {          //처음 쐈을 때 총알의 좌표 초기화
            bull[i].bx = x + 2;
            bull[i].by = y;
            bull[i].bullet_shot = FALSE;
        }

        if (bull[i].bullet_exist && bull[i].b_speed % BULLET_SPEED == 1) {  //총알의 속도 조절
            bull[i].by--;
            if (bull[i].by < 0) bull[i].bullet_exist = FALSE;
            else {
                ScreenPrint(bull[i].bx, bull[i].by, "||");
            }
        }
        ScreenPrint(bull[i].bx, bull[i].by, "||");
        bull[i].b_speed++;
    }
}

//비행기 출력
void Render(int key, char(*string)[7])
{
    //비활성 버퍼를 지움
    ScreenClear();

    switch (key) {
    case RIGHT:
        if (x < 63) x++;
        break;
    case LEFT:
        if (x > 0) x--;
        break;
    case UP:
        if (y > 0) y--;
        break;
    case DOWN:
        if (y < 45) y++;
        break;
    default:
        break;
    }
    ScreenPrint(x, y, string[0]);
    ScreenPrint(x, y + 1, string[1]);
}