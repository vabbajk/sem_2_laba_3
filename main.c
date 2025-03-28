#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include "Error_codes.h"
#include "byte_field_task.h"
#include "INTERFACE.h"
#include "BANK_SYSTEM_TASK.h"
#include "MAIN_DATABASE_FUNCTIONS.h"
#include <string.h>

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
                bite_field();
            case '2':
                bank_system();
            default:
                loop = 0;
                break;
        }


    }

    system("pause");
}