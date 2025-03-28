#ifndef BANK_SYSTEM_TASK_H
#define BANK_SYSTEM_TASK_H

#include "MAIN_DATABASE_FUNCTIONS.h"
#include "INTERFACE.h"


int bank_system() {


    setlocale(LC_ALL, "");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    struct client *clients = NULL;

    struct data_base db;

    read_clients_from_file(&db, &clients, CLIENT_BASE);

    puts("\nВ этом задании представлена база данных на основе структур.\n");


    int a = 1;
    while (a) {
        print_menu();
        printf(" > ");
        rewind(stdin);
        char ch = choce_task_n_to_m('0', '8');

        printf(" %c \n", ch);

        switch (ch) {
            case '1':
                add_new_client(&clients, &db);
                break;
            case '2':
                look_all_clients(clients, db);
                break;
            case '3':
                add_new_account(&clients, &db);
                break;
            case '4':
                find_all_accounts(clients, db);
                break;
            case '5':
                delete_clients(&clients, &db);
                break;
            case '6':
                create_db(&db);
                break;
                break;
            case '0':
                a = 0;
                break;

        }

        rewrite_all_clients(&db, clients, CLIENT_BASE);

        system("pause");

        rewind(stdin);

    }

}

#endif
