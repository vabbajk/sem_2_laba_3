#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include "Error_codes.h"
#include "task_1.h"
#include "functions_to_interface.h"
#include "task_2.h"
#include "functions_to_task_2.h"
#include <string.h>

struct First_task_struct {
    unsigned int number:32;
};

int main(void) {


    setlocale(LC_ALL, "russian");

    int loop = 1;

    while (loop) {
            print_lab3_menu();

            char c = choce_task_n_to_m('1', '3');

            printf("\033[6;30H%c", c);

            printf("\033[6;0H\n\n");

        switch (c) {
            case '1':
                task_1();
            case '2':
                task_2();
            default:
                loop = 0;
                break;
        }


    }

    system("pause");
}