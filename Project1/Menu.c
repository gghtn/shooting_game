#include "Menu.h"
#include <stdio.h>

#define UP 72
#define DOWN 80
#define MAX_LEVEL 10

extern int status[5];

//���θ޴� ���ٿ�
void main_menu(int key, int* x, int* y) {
	if (key == UP) {
        gotoxy(*x, *y);
        printf("  ");
        if (*y > 10) *y -= 5;
        gotoxy(*x, *y);
        printf("��");
	}
    else if (key == DOWN) {
        gotoxy(*x, *y);
        printf("  ");
        if (*y < 20) *y += 5;
        gotoxy(*x, *y);
        printf("��");
    }
}

//�������ͽ� ���
void status_print() {
    system("cls");
    gotoxy(30, 8);
    printf("Lv %7d", status[0]);
    gotoxy(30, 9);
    printf("Power %4d", status[1]);
    gotoxy(30, 10);
    printf("Speed %4d", status[2]);
    gotoxy(30, 11);
    printf("Life %5d", status[3]);
    gotoxy(30, 12);
    printf("Gold %5d", status[4]);

    gotoxy(27, 15);
    printf("��");
    gotoxy(30, 15);
    printf("���� ��!!(10G)");
    gotoxy(30, 20);
    printf("���� �޴�");
}

//�������ͽ� �޴� ���ٿ�
void status_menu(int key, int* x, int* y) {
    if (key == UP) {
        gotoxy(*x, *y);
        printf("  ");
        if (*y > 15) *y -= 5;
        gotoxy(*x, *y);
        printf("��");
    }
    else if (key == DOWN) {
        gotoxy(*x, *y);
        printf("  ");
        if (*y < 20) *y += 5;
        gotoxy(*x, *y);
        printf("��");
    }
}

//���� ��
void LvelUp(int y) {
    if (y == 15 && status[4] >= 10 && status[0] < MAX_LEVEL) {
        status[4] -= 10;
        for (int i = 0; i < 3; i++) {  
            status[i]++;
        }
        if (status[3] < 7) status[3]++; // ���� �ִ� 7
        system("cls");
        status_print();
    }
}