#ifndef FUNCTIONS_TO_TASK_2_H
#define FUNCTIONS_TO_TASK_2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <limits.h>
#include <time.h>

#define CLIENT_BASE "clients_base.bin"


struct account {
    char account_number[8];
    signed int balance;
};

struct client {
    signed int passport_num;
    signed int last_name_length;
    char* last_name;
    signed int account_count;
    struct account* account;
};

struct data_base {
    int record_amount;
};

void create_db(struct data_base* db) {
    FILE *file = fopen(CLIENT_BASE, "wb");
    if (file == NULL) {
        perror("Ошибка открытия файла clients_base.bin");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    if (file_size != 0) {
        fprintf(stderr, "Файл clients_base.bin не пуст. База данных не была перезаписана.\n");
        fclose(file);
        return;
    }

    db->record_amount = 0;
    rewind(file);

    if (fwrite(db, sizeof(struct data_base), 1, file) != 1) {
        perror("Ошибка записи в файл");
        fclose(file);
        return;
    }

    fclose(file);
}

int get_record_amount() {
    FILE *file = fopen(CLIENT_BASE, "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла для чтения (get_record_amount)");
        return 0;
    }
    struct data_base db;
    if (fread(&db, sizeof(db), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    fclose(file);
    return db.record_amount;
}

int check_passport_num_to_duplicate(struct client *clients, struct data_base db, int passport_num) {
    for (int i = 0; i < db.record_amount; i++) {
        if (clients[i].passport_num == passport_num) { printf("\n \033[0m! ! ! Клиент с номером паспорта %d уже зарегестрирован ! ! !\033[0m\n\n", passport_num); return 1;}
    }
    return 0;
}

int rewrite_all_clients(const struct data_base *db, const struct client *clients, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Ошибка открытия файла для записи");
        return 1;
    }

    if (fwrite(db, sizeof(struct data_base), 1, file) != 1) {
        perror("Ошибка записи метаданных");
        fclose(file);
        return 2;
    }

    for (int i = 0; i < db->record_amount; i++) {
        const struct client *c = &clients[i];

        if (fwrite(&c->passport_num, sizeof(c->passport_num), 1, file) != 1) {
            printf("Ошибка записи паспорта клиента %d.\n", i);
            fclose(file);
            return 3;
        }

        if (fwrite(&c->last_name_length, sizeof(c->last_name_length), 1, file) != 1) {
            printf("Ошибка записи длины фамилии клиента %d.\n", i);
            fclose(file);
            return 4;
        }

        if (fwrite(c->last_name, 1, c->last_name_length, file) != (size_t)c->last_name_length) {
            printf("Ошибка записи фамилии клиента %d.\n", i);
            fclose(file);
            return 5;
        }

        if (fwrite(&c->account_count, sizeof(c->account_count), 1, file) != 1) {
            printf("Ошибка записи количества счетов клиента %d.\n", i);
            fclose(file);
            return 6;
        }

        for (int j = 0; j < c->account_count; j++) {
            const struct account *acc = &c->account[j];

            if (fwrite(acc->account_number, 1, sizeof(acc->account_number), file)
                != sizeof(acc->account_number)) {
                fclose(file);
                return 7;
            }

            if (fwrite(&acc->balance, sizeof(acc->balance), 1, file) != 1) {
                fclose(file);
                return 8;
            }

        }
    }

    fclose(file);
    return 0;
}

int read_clients_from_file(struct data_base *db, struct client **clients, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }

    if (fread(db, sizeof(struct data_base), 1, file) != 1) {
        fclose(file);
        printf("Ошибка чтения метаданных.\n");
        return 2;
    }

    int record_amount = db->record_amount;
    *clients = (struct client*)malloc(record_amount * sizeof(struct client));
    if (!*clients) {
        fclose(file);
        printf("Ошибка выделения памяти для клиентов.\n");
        return 3;
    }

    int current_i = 0;
    for (current_i = 0; current_i < record_amount; current_i++) {
        struct client *current = &(*clients)[current_i];

        if (fread(&current->passport_num, sizeof(current->passport_num), 1, file) != 1) {
            printf("Ошибка чтения паспорта клиента %d.\n", current_i);
            goto error;
        }

        if (fread(&current->last_name_length, sizeof(current->last_name_length), 1, file) != 1) {
            printf("Ошибка чтения длины фамилии клиента %d.\n", current_i);
            goto error;
        }

        current->last_name = (char*)malloc((current->last_name_length + 1) * sizeof(char));
        if (!current->last_name) {
            goto error;
        }

        if (fread(current->last_name, 1, current->last_name_length, file)
            != (size_t)current->last_name_length) {
            goto error;
        }
        current->last_name[current->last_name_length] = '\0';


        if (fread(&current->account_count, sizeof(current->account_count), 1, file) != 1) {
            printf("Ошибка чтения количества счетов клиента %d.\n", current_i);
            goto error;
        }


        current->account = (struct account*)malloc(current->account_count * sizeof(struct account));
        if (!current->account) {
            goto error;
        }

        for (int j = 0; j < current->account_count; j++) {
            struct account *acc = &current->account[j];


            if (fread(acc->account_number, 1, sizeof(acc->account_number), file)
                != sizeof(acc->account_number))
                {
                goto error;
                }

            acc->account_number[sizeof(acc->account_number) - 1] = '\0';

            if (fread(&acc->balance, sizeof(acc->balance), 1, file) != 1) {
                goto error;
            }

        }
    }

    fclose(file);
    return 0;

error:

    for (int i = 0; i < current_i; i++) {
        free((*clients)[i].last_name);
        free((*clients)[i].account);
    }
    free(*clients);
    *clients = NULL;
    fclose(file);
    printf("Ошибка чтения данных на клиенте %d.\n", current_i);
    return 4;
}

int write_client_to_file(const struct client *new_client, const char *filename)
{
    setlocale(LC_ALL, "");

    struct data_base db;
    struct client *clients = NULL;


    if (read_clients_from_file(&db, &clients, filename) != 0) {
        db.record_amount = 0;
        clients = NULL;
    }

    int old_count = db.record_amount;
    int new_count = old_count + 1;

    struct client *temp = realloc(clients, new_count * sizeof(struct client));
    if (!temp) {
        if (clients) {
            for (int i = 0; i < old_count; i++) {
                free(clients[i].last_name);
                free(clients[i].account);
            }
            free(clients);
        }
        return 1;
    }
    clients = temp;

    clients[old_count].passport_num     = new_client->passport_num;
    clients[old_count].last_name_length = new_client->last_name_length;

    clients[old_count].last_name = malloc(new_client->last_name_length * sizeof(char));
    if (!clients[old_count].last_name) {
        for (int i = 0; i < old_count; i++) {
            free(clients[i].last_name);
            free(clients[i].account);
        }
        free(clients);
        return 1;
    }
    memcpy(clients[old_count].last_name,
           new_client->last_name,
           new_client->last_name_length);

    clients[old_count].account_count = new_client->account_count;
    clients[old_count].account = malloc(new_client->account_count * sizeof(struct account));
    if (!clients[old_count].account) {
        free(clients[old_count].last_name);
        for (int i = 0; i < old_count; i++) {
            free(clients[i].last_name);
            free(clients[i].account);
        }
        free(clients);
        return 1;
    }
    memcpy(clients[old_count].account,
           new_client->account,
           new_client->account_count * sizeof(struct account));

    db.record_amount = new_count;

    if (rewrite_all_clients(&db, clients, filename) != 0) {
        for (int i = 0; i < new_count; i++) {
            free(clients[i].last_name);
            free(clients[i].account);
        }
        free(clients);
        return 1;
    }

    for (int i = 0; i < new_count; i++) {
        free(clients[i].last_name);
        free(clients[i].account);
    }
    free(clients);

    return 0;
}

int add_new_client(struct client **clients, struct data_base *db) {
    setlocale(LC_ALL, "russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    int passport_num = 0;
    int is_first_iteration = 1;

    do {
        system("cls || clear");
        printf("\033[1;36m********************************************\033[0m\n");
        printf("\033[1;36m*\033[1;35m         ДОБАВЛЕНИЕ НОВОГО КЛИЕНТА        \033[1;36m*\033[0m\n");
        printf("\033[1;36m********************************************\033[0m\n");

        if (!is_first_iteration) {
            printf("\033[1;36m*\033[1;31m Ошибка: номер паспорта уже существует!   \033[1;36m*\033[0m\n");
            printf("\033[1;36m********************************************\033[0m\n");
        }

        printf("\033[1;36m*\033[1;33m Введите номер паспорта:                  \033[1;36m*\033[0m\n");
        printf("\033[1;36m********************************************\033[0m\n");
        printf("\033[5m_\033[0m ");

        passport_num = new_input_metod(0, INT_MAX);

        rewind(stdin);
        if (passport_num == -1) {return 0;}
        is_first_iteration = 0;
    } while (check_passport_num_to_duplicate(*clients, *db, passport_num));

    system("cls || clear");
    printf("\033[1;36m********************************************\033[0m\n");
    printf("\033[1;36m*\033[1;35m         ДОБАВЛЕНИЕ НОВОГО КЛИЕНТА        \033[1;36m*\033[0m\n");
    printf("\033[1;36m********************************************\033[0m\n");
    printf("\033[1;36m*\033[1;33m Введите фамилию:                         \033[1;36m*\033[0m\n");
    printf("\033[1;36m********************************************\033[0m\n");
    printf("\033[5m_\033[0m ");

    struct client new_client;
    input_only_letters(&new_client.last_name, &new_client.last_name_length);
    rewind(stdin);

    new_client.passport_num = passport_num;
    new_client.account_count = 0;
    new_client.account = (struct account*)malloc(0);

    int current_count = db->record_amount;

    *clients = realloc(*clients, (current_count + 1) * sizeof(struct client));
    if (*clients == NULL) {
        free(new_client.last_name);
        free(new_client.account);
        return -1;
    }

    (*clients)[current_count] = new_client;
    db->record_amount = current_count + 1;

    return 505;
}

void free_clients(struct client *clients, int count) {
    for (int i = 0; i < count; i++) {
        free(clients[i].last_name);
        free(clients[i].account);
    }
    free(clients);
}

char* add_account_to_client(struct client **clients, struct data_base *db,
                            const char *account, int passport_num, int balance)
{
    int client_index = -1;

    for (int i = 0; i < db->record_amount; i++) {
        if ( (*clients)[i].passport_num == passport_num ) {
            client_index = i;
            break;
        }
    }

    if (client_index == -1) {
        return "2";
    }

    struct client *target = &(*clients)[client_index];


    for (int i = 0; i < target->account_count; i++) {
        if (strcmp(target->account[i].account_number, account) == 0) {
            return "4";
        }
    }

    target->account_count++;

    struct account *tmp = (struct account*)realloc(target->account,
                                target->account_count * sizeof(struct account));
    if (!tmp) {
        return "3";
    }
    target->account = tmp;

    struct account *new_acc = &target->account[target->account_count - 1];

    strncpy(new_acc->account_number, account, 7);
    new_acc->account_number[7] = '\0';

    new_acc->balance = balance;

    return target->last_name;
}


int look_all_clients(struct client *clients, struct data_base db) {

    printf("\033[0m");

    if (db.record_amount == 0) {
        printf("\033[1;33mПока что у вас нет клиентов!\033[0m\n");
        return 1;
    }

    printf("\n\033[1;36m============= СПИСОК КЛИЕНТОВ =============\033[0m\n");
    printf("\033[1;37mВсего клиентов: %d\033[0m\n\n", db.record_amount);

    for (int i = 0; i < db.record_amount; i++) {
        struct client *client = &clients[i];

        printf("\033[1;34m+---------------------------------------+\033[0m\n");
        printf("\033[1;34m| Клиент №%-30d|\033[0m\n", i + 1);
        printf("\033[1;34m+-------------------+-------------------+\033[0m\n");

        printf("\033[1;37m| Паспорт:          | %-17d |\033[0m\n", client->passport_num);
        printf("\033[1;37m| Фамилия:          | %-17s |\033[0m\n", client->last_name);
        printf("\033[1;37m| Количество счетов:| %-17d |\033[0m\n", client->account_count);
        printf("\033[1;34m+-------------------+-------------------+\033[0m\n");

        if (client->account_count > 0) {
            printf("\033[1;37m| Счета:                                |\033[0m\n");
            printf("\033[1;34m+-------------------+-------------------+\033[0m\n");

            for (int j = 0; j < client->account_count; j++) {
                struct account *acc = &client->account[j];
                printf("\033[1;37m| Номер:            | %-17s |\033[0m\n", acc->account_number);
                printf("\033[1;37m| Баланс:           | %-17d |\033[0m\n", acc->balance);
                if (j < client->account_count - 1) {
                    printf("\033[1;34m+-------------------+-------------------+\033[0m\n");
                }
            }
        }

        printf("\033[1;34m+---------------------------------------+\033[0m\n\n");
    }

    return 0;
}

int add_new_account(struct client **clients, struct data_base *db) {
    system("cls");
    printf(BORDER "\n========================================\n" RESET);
    printf(YELLOW "Введите номер паспорта: " RESET);
    int passport_num = new_input_metod(INT_MIN, INT_MAX);

    printf(BORDER "----------------------------------------\n" RESET);
    printf(YELLOW "Введите номер счета: " RESET);
    char account_number[8];
    input_only_letters_and_numbers(account_number, 8);

    printf(BORDER "----------------------------------------\n" RESET);
    printf(YELLOW "Введите начальный баланс: " RESET);
    int balance = new_input_metod(INT_MIN, INT_MAX);

    printf(BORDER "========================================\n" RESET);

    char* last_name = add_account_to_client(clients, db, account_number, passport_num, balance);

    if (strcmp(last_name, "2") == 0) {
        printf(BORDER "========================================\n" RESET);
        printf(RED "! ! ! Клиент с паспортом %d не найден ! ! !\n" RESET, passport_num);
        printf(BORDER "========================================\n\n" RESET);
        return 2;
    } else if (strcmp(last_name, "3") == 0) {
        printf(BORDER "========================================\n" RESET);
        printf(RED "Ошибка памяти при добавлении счёта!\n" RESET);
        printf(BORDER "========================================\n\n" RESET);
        return 3;
    }else if (strcmp(last_name, "4") == 0) {
        printf(BORDER "========================================\n" RESET);
        printf(RED"Счет с таким названием уже существует!\n" RESET);
        printf(BORDER "========================================\n\n" RESET);
        return 3;
    }

    system("cls");


    printf(GREEN "\n\nСчет " YELLOW "%s" GREEN " с балансом " YELLOW "%d" GREEN " успешно добавлен клиенту " YELLOW "%s" GREEN "!\n\n\n" RESET,
           account_number, balance, last_name);


    return 0;
}

int delete_clients(struct client **clients, struct data_base *db) {
    char* last_name_to_delete;
    int last_name_length = 0;

    printf("\033[1;36mВведите фамилию для удаления: \033[0m");
    input_only_letters(&last_name_to_delete, &last_name_length);

    printf("\n\033[1;33mФамилия для удаления: \033[1;37m%s\033[0m\n", last_name_to_delete);
    printf("\033[1;33mКоличество записей в базе: \033[1;37m%d\033[0m\n", db->record_amount);

    int keep_count = 0;
    for (int i = 0; i < db->record_amount; i++) {
        if (strcmp((*clients)[i].last_name, last_name_to_delete) != 0) {
            keep_count++;
        }
    }

    if (keep_count == db->record_amount) {
        printf("\n\033[1;31mКлиенты с фамилией '\033[1;37m%s\033[1;31m' не найдены!\033[0m\n\n", last_name_to_delete);
        free(last_name_to_delete);
        return 2;
    }

    printf("\n\033[1;34m============================================\033[0m\n");
    printf("\033[1;34m|         УДАЛЕНИЕ КЛИЕНТОВ ИЗ БАЗЫ        |\033[0m\n");
    printf("\033[1;34m============================================\033[0m\n");

    int new_record_amount = db->record_amount;
    for (int i = 0; i < new_record_amount; i++) {
        if (strcmp((*clients)[i].last_name, last_name_to_delete) == 0) {
            printf("\033[1;31mУдаление клиента: \033[1;37m%s\033[0m\n", (*clients)[i].last_name);

            free((*clients)[i].last_name);
            free((*clients)[i].account);

            for (int j = i; j < new_record_amount - 1; j++) {
                (*clients)[j] = (*clients)[j + 1];
            }

            new_record_amount--;
            i--;
        }
    }

    *clients = realloc(*clients, new_record_amount * sizeof(struct client));
    if (*clients == NULL && new_record_amount > 0) {
        printf("\n\033[1;31mОшибка при перераспределении памяти!\033[0m\n");
        free(last_name_to_delete);
        return -1;
    }

    db->record_amount = new_record_amount;

    printf("\n\033[1;32mКлиенты с фамилией '\033[1;37m%s\033[1;32m' успешно удалены!\033[0m\n", last_name_to_delete);
    printf("\033[1;34m============================================\033[0m\n\n");

    free(last_name_to_delete);

    return 0;
}

int find_all_accounts(struct client *clients, struct data_base db) {

    printf("\033[0m");

    printf("\n\033[1;36m=== Поиск счетов клиента ===\033[0m\n\n");


    printf("Введите номер паспорта: ");
    int passport_num = new_input_metod(0, INT_MAX);

    int client_index = -1;
    for (int i = 0; i < db.record_amount; i++) {
        if (clients[i].passport_num == passport_num) {
            client_index = i;
            break;
        }
    }

    if (client_index == -1) {
        printf("\033[1;31mОшибка: Клиент с паспортом %d не найден!\033[0m\n", passport_num);
        return 2;
    }

    if (clients[client_index].account_count == 0) {
        printf("\033[1;33mКлиент с паспортом %d не имеет активных счетов.\033[0m\n", passport_num);
    }
    else {
        printf("\n\033[1;32mКлиент: %s (паспорт: %d)\033[0m\n", clients[client_index].last_name, passport_num);
        printf("\033[1;32mКоличество счетов: %d\033[0m\n", clients[client_index].account_count);

        printf("\n\033[1;34m+----------------------+-------------+\033[0m\n");
        printf("\033[1;34m| %-20s | %-11s |\033[0m\n", "Номер счета", "Баланс");
        printf("\033[1;34m+----------------------+-------------+\033[0m\n");

        for (int i = 0; i < clients[client_index].account_count; i++) {
            printf("\033[1;34m| \033[0m%-20s \033[1;34m|\033[0m %-11d \033[1;34m|\033[1;34m\n",
                   clients[client_index].account[i].account_number,
                   clients[client_index].account[i].balance);
        }

        printf("\033[1;34m+----------------------+-------------+\033[0m\n");
    }


    return 0;
}

int initialize_obligatory_task(struct client **clients, struct data_base *db) {
    srand((unsigned)time(NULL));
    *clients = NULL;
    db->record_amount = 10;
    for (int i = 0; i < 10; i++) {
        struct client c;
        c.passport_num = rand() % 1000000;
        c.last_name_length = rand() % 15 + 1;

        c.last_name = (char*)malloc(c.last_name_length + 1);
        for (int j = 0; j < c.last_name_length; j++) {
            c.last_name[j] = (char)('а' + (rand() % 32));
        }
        c.last_name[c.last_name_length] = '\0';

        c.account_count = rand() % 10;
        c.account = (struct account*)malloc(c.account_count * sizeof(struct account));

        for (int k = 0; k < c.account_count; k++) {
            for (int t = 0; t < 7; t++) {
                c.account[k].account_number[t] = (char)('A' + (rand() % 26));
            }
            c.account[k].account_number[7] = '\0';
            c.account[k].balance = rand() % 10000;
        }

        *clients = (struct client*)realloc(*clients, (i + 1) * sizeof(struct client));
        (*clients)[i] = c;
    }
    return 0;
}


#endif // FUNCTIONS_TO_TASK_2_H