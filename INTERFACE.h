#ifndef FUNCTIONS_TO_INTERFACE_H
#define FUNCTIONS_TO_INTERFACE_H
#include <stdio.h>
#include <conio.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"
#define BORDER "\033[1;36m"

char choce_task() {
    char c = 'l';
    while (c < '1' || c > '3') {
        c = _getch();
    }
    return c;
}

char choce_task_n_to_m(char n, char m) {
    char c = 'l';
    while (c < n || c > m) {
        c = _getch();
    }
    return c;
}

void print_menu() {
    system("cls || clear");

    printf("\033[1;36m********************************************\033[0m\n");
    printf("\033[1;36m*\033[1;35m           ���������� �������             \033[1;36m*\033[0m\n");
    printf("\033[1;36m********************************************\033[0m\n");


    printf("\033[1;36m*\033[1;33m 1\033[0m - �������� ������ ������� � ��         \033[1;36m*\033[0m\n");
    printf("\033[1;36m*\033[1;33m 2\033[0m - ����������� ������ ���� ��������     \033[1;36m*\033[0m\n");
    printf("\033[1;36m*\033[1;33m 3\033[0m - �������� ���� �������                \033[1;36m*\033[0m\n");
    printf("\033[1;36m*\033[1;33m 4\033[0m - ������ ���������� ������ �������     \033[1;36m*\033[0m\n");
    printf("\033[1;36m*\033[1;33m 5\033[0m - ������� �������� � �������� �������� \033[1;36m*\033[0m\n");
    printf("\033[1;36m*\033[1;33m 6\033[0m - ������������������� ��               \033[1;36m*\033[0m\n");
    printf("\033[1;36m*\033[1;33m 7\033[0m - ������������� ��������� ��           \033[1;36m*\033[0m\n");
    printf("\033[1;36m*\033[1;31m 0\033[0m - �����                                \033[1;36m*\033[0m\n");


    printf("\033[1;36m********************************************\033[0m\n");
    printf("\033[1;36m*\033[1;37m ������� ����� ��������: \033[30m                \033[1;36m *\033[0m\n");
    printf("\033[1;36m********************************************\033[0m\n");
    printf("\033[5m_\033[0m ");
}

void print_lab3_menu() {
    system("cls || clear");

    printf("\033[1;36m======================================================\033[0m\n");

    printf("\033[1;36m||\033[1;35m �� ���������� � ���� ������� ������������ ������ \033[1;36m||\033[0m\n");
    printf("\033[1;36m||\033[1;37m                                                  \033[1;36m||\033[0m\n");

    printf("\033[1;36m||\033[1;33m �������� ��������� (1-2) ��� 0 ��� ������        \033[1;36m||\033[0m\n");
    printf("\033[1;36m||\033[1;37m                                                  \033[1;36m||\033[0m\n");

    printf("\033[1;36m||\033[1;32m �� ������� ������� �����                         \033[1;36m||\033[0m\n");

    printf("\033[1;36m======================================================\033[0m\n");

    printf("\033[5m������� ����� ���������: \033[0m");
}



#endif
