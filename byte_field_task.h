#ifndef BYTE_FIELD_TASK
#define BYTE_FIELD_TASK



#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <conio.h>
#include "Header.h"

struct thirty_two_bit_num_struct {
    long long int number:64;
};

int bite_field() {

    setlocale(LC_ALL, "russian");

    struct thirty_two_bit_num_struct s;

    puts("������� : ����� � ������� ����� ����� � 2 ���� ������ ���������,"
         "\n�� �������� ������� � �� ��������� �������� ������ ������� �� �������. \n"
         "����������, ������� ����� (-9,223,372,036,854,775,808 < num < 9,223,372,036,854,775,807\n");

    long long int temp;

    temp = new_input_metod(LONG_MIN, LONG_MAX);

    s.number = temp;

    printf("��� ����������� ��������� ������� %lld �� 2 : %lld\n",temp, s.number >> 1);

    return 505;
}



#endif
