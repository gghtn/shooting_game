#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "Console.h"
#include "DoubleBuffering.h"
#include "Menu.h"
#include "SaveFile.h"
#pragma warning(disable:4996)

#define TRUE 1
#define FALSE 0
#define BULLET_SPEED 15
#define BULLET_COUNT 10
#define ENEMY_SPEED 10
#define ENEMY_COUNT 50
#define FILE_NAME "savefile.bin"

enum { UP = 72, DOWN = 80, ENTER = 13, ESC = 27, SPACE = 32, YES = 121 };
int x, y; //유저 좌표

typedef int BOOL;

typedef struct {
    BOOL bullet_exist;  //총알의 존재여부
    BOOL bullet_shot;   //발사 여부
    int bx, by;         //총알의 좌표
    int bcount;         //속도 결정을 위한 변수
}Bullet;

typedef struct {
    BOOL enemy_live;    //적기 생존여부
    int ex, ey;         //적기 좌표
    int ecount;         //속도 결정 변수
    int espeed;         //적기 속도
    int ehp;            //적기 hp
}Enemy;

typedef struct {
    int Lv, power, speed, life, gold;
}User;

//버퍼에 출력하는 함수
void RendBulletEnemy(Bullet* bull, Enemy* enemy, User* user);
void BulletCrash(Bullet* bull, Enemy* enemy, User* user);
void GameStart();
void GameOver();

int score, splash;  //점수, 격추 수
int status[5] = { 1, 5, 1, 3, 0 }; // 레벨, 파워, 총알 스피드, 생명, 골드

int main() {
    srand((unsigned)time(NULL));
	setConsoleSize(70, 50);

    FILE *in;
    in = fopen(FILE_NAME, "rb");
    if(in == NULL){}
    else {
        printf("저장파일을 불러오겠습니까?(Y/N)");
        if (getch() == YES) {
            LoadFile(in);
            fclose(in);
        }
    }

    while (1) {
        CursorView(); //커서 숨기기
        int menu_x = 27, menu_y = 10;
        system("cls");
        gotoxy(menu_x, menu_y);
        printf("▶");
        gotoxy(30, 10);
        printf("게임 시작");
        gotoxy(30, 15);
        printf("유저 정보");
        gotoxy(30, 20);
        printf("게임 종료");

        while (1) {
            if (kbhit()) {              //키 입력이 있을 시
                int key = getch();
                if (key == ENTER) {
                    if (menu_y == 10) {
                        system("cls");
                        score = 0, splash = 0;
                        GameStart();
                        break;
                    }
                    else if (menu_y == 15) {

                        status_print();
                        while (1) {
                            if (kbhit()) {
                                int s_key = getch();
                                if (s_key == ENTER) {
                                    LvelUp(menu_y);
                                    if (menu_y == 20) break;
                                }
                                else status_menu(s_key, &menu_x, &menu_y);
                            }
                        }
                        break;

                    }
                    else {                              //메뉴에서 게임 종료 선택시
                        system("cls");
                        printf("게임 종료");
                        SaveFile();                  //스테이터스 저장
                        return 0;
                    }
                }
                else {
                    main_menu(key, &menu_x, &menu_y);   //메인 메뉴 up down함수
                    if (key == ESC) {                   //메인에서 ESC클릭시 게임 종료
                        system("cls");
                        printf("게임 종료");
                        SaveFile();
                        return 0;
                    }
                }
            }
        }
        
    }

	return 0;
}

void GameStart() {
    //화면 버퍼 생성, 초기화
    ScreenInit();
    x = 32, y = 45;

    char str_score[100];    //점수 출력문을 저장할 문자열
    char user_life[100];    //유저생명 출력문 저장할 문자열
    char str_splash[100];   //처치 수
    char str[2][7] = { "  △  ", "◁■▷" };

    User user = { status[0], status[1], status[2], status[3], status[4] }; //유저 스탯 초기화

    //총알 동적할당, 초기화
    Bullet* bull = (Bullet*)malloc(sizeof(Bullet) * BULLET_COUNT);
    if (bull == NULL) {
        ScreenRelease();
        printf("오류발생");
    }
    for (int i = 0; i < BULLET_COUNT; i++) {
        bull[i].bullet_exist = FALSE;
        bull[i].bullet_shot = FALSE;
    }

    //적기 동적할당, 초기화
    Enemy* enemy = (Enemy*)malloc(sizeof(Enemy) * ENEMY_COUNT);
    if (enemy == NULL) {
        ScreenRelease();
        printf("오류발생");
    }
    for (int i = 0; i < ENEMY_COUNT; i++) {
        enemy[i].enemy_live = FALSE;
        enemy[i].ehp = 15;
    }

    int count = 0; //유저 속도조절 변수
    while (1) {
        if (user.life <= 0) {
            GameOver();
            break;
        }
        //비활성 버퍼를 지움
        ScreenClear();

        count++;                            //점수,유저 속도조절 변수
        if (count % 100 == 0) score++;      //시간이 지날수록 점수 증가
        if (count % 10 == 0) {
            if (IsKeyDown(VK_LEFT)) {
                if (x > 0) x--;
            }
            if (IsKeyDown(VK_RIGHT)) {
                if (x < 63) x++;
            }
            if (IsKeyDown(VK_UP)) {
                if (y > 0) y--;
            }
            if (IsKeyDown(VK_DOWN)) {
                if (y < 45) y++;
            }
        }
        //점수 출력
        sprintf(str_score, "score : %d", score);
        ScreenPrint(1, 0, str_score);

        //처치 수 출력
        sprintf(str_splash, "처치 수: %d", splash);
        ScreenPrint(1, 1, str_splash);

        //남은 생명 출력
        sprintf(user_life, "Life ");
        for (int i = 0; i < user.life; i++) {
            sprintf(user_life, "%s■", user_life);
        }
        ScreenPrint(1, 2, user_life);

        //아군 출력
        ScreenPrint(x, y, str[0]);
        ScreenPrint(x, y + 1, str[1]);

        if (kbhit()) {
            int key = getch();
            if (key == ESC) {
                GameOver();
                break;
            }
            else if (key == SPACE) {
                for (int i = 0; i < BULLET_COUNT; i++) {   //i번째 총알이 존재하지 않으면 발사
                    if (!bull[i].bullet_exist) {
                        bull[i].bullet_exist = TRUE;
                        bull[i].bullet_shot = TRUE;
                        bull[i].bcount = 0;
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < ENEMY_COUNT; i++) {         //i번째 적이 존재하지 않으면 적생성,초기화
            if (!enemy[i].enemy_live && rand() % 10 == 0) {
                enemy[i].enemy_live = TRUE;
                enemy[i].ecount = 0;
                enemy[i].espeed = ENEMY_SPEED + rand() % 100;
                enemy[i].ex = rand() % 68 + 2;
                enemy[i].ey = rand() % 10 + 3;
            }
        }
        RendBulletEnemy(bull, enemy, &user);

        //버퍼교체
        ScreenFlipping();
    }
    free(bull);
    free(enemy);
    //system("pause");
}

void GameOver() {
    score += (splash * 10);
    ScreenRelease();
    gotoxy(30, 15);
    printf("게임오버");
    gotoxy(28, 17);
    printf("처치 수: %d", splash);
    gotoxy(28, 19);
    printf("총 점수: %d", score);
    status[4] += score / 100;
    _sleep(3000);
}

//적기, 총알 출력
void RendBulletEnemy(Bullet* bull, Enemy* enemy, User* user) {
    for (int i = 0; i < BULLET_COUNT; i++) {
        if (bull[i].bullet_shot) {          //처음 쐈을 때 총알의 좌표 초기화
            bull[i].bx = x + 2;
            bull[i].by = y - 1;
            bull[i].bullet_shot = FALSE;
        }

        if (bull[i].bullet_exist) {
            if (bull[i].bcount % (BULLET_SPEED-status[2]) == 1) {  //총알의 속도 조절
                bull[i].by--;
                if (bull[i].by < 3) bull[i].bullet_exist = FALSE;
                else {
                    ScreenPrint(bull[i].bx, bull[i].by, "||");
                }
            }
            else ScreenPrint(bull[i].bx, bull[i].by, "||");
            bull[i].bcount++;
        }
    }

    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (enemy[i].enemy_live) {
            //적기가 움직일 경우
            if (enemy[i].ecount % enemy[i].espeed == 1) {   //적기 속도조절
                BulletCrash(bull, &enemy[i], user);
                if (enemy[i].enemy_live) {
                    enemy[i].ey++;
                    if(enemy[i].ey == 46) enemy[i].enemy_live = FALSE;
                    else ScreenPrint(enemy[i].ex, enemy[i].ey,"@");
                }
            }
            //적기가 움직이지 않은 경우
            else {
                BulletCrash(bull, &enemy[i], user);
                if (enemy[i].enemy_live)
                    ScreenPrint(enemy[i].ex, enemy[i].ey, "@");
            }
            enemy[i].ecount++;
        }
    }
}

//적기 격추시
void BulletCrash(Bullet* bull, Enemy* enemy, User* user) {
    for (int j = 0; j < BULLET_COUNT && bull[j].bullet_exist; j++) {
        if (abs(enemy->ex - bull[j].bx) <= 1 && abs(enemy->ey - bull[j].by) <= 1) { //적이 총알에 맞을 경우
            enemy->ehp -= status[1];                                                //적 HP가 유저의 Power스탯만큼 깍임
            bull[j].bullet_exist = FALSE;                                           //충돌 총알 삭제
            ScreenPrint(bull[j].bx, bull[j].by, "  ");

            if (enemy->ehp <= 0) {
                enemy->enemy_live = FALSE;
                splash += 1;
            }
            break;
        }
    }
    if (enemy->ex - x >= 0 && enemy->ex - x <= 6 && enemy->ey - y == 0) {           //적이 유저와 충돌 시
        enemy->enemy_live = FALSE;
        user->life--;
    }
}

