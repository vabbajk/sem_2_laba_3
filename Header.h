#ifndef HEADER_H
#define HEADER_H
#define CLIENT_BASE "clients_base.bin"
#include <locale.h>


int validate_and_parse_int(const char *input, int *output) {
    if (input == NULL || *input == '\0') {
        return 0;
    }

    const char *ptr = input;
    int is_negative = 0;

    if (*ptr == '+' || *ptr == '-') {
        is_negative = (*ptr == '-');
        ptr++;
    }

    if (*ptr == '\0') {
        return 0;
    }

    long result = 0;
    while (*ptr != '\0') {
        if (*ptr < '0' || *ptr > '9') {
            return 0;
        }

        int digit = *ptr - '0';

        if ((!is_negative && result > (LONG_LONG_MAX - digit) / 10) ||
            (is_negative && -result < (-LONG_LONG_MIN + digit) / 10)) {
            return 0;
        }

        result = result * 10 + digit;
        ptr++;
    }

    *output = is_negative ? -(int)result : (int)result;
    return 1;
}

int check_int_1(char *input, int min, int max) {
    if (input == NULL || *input == '\0') {
        return 0;
    }
    const char *ptr = input;
    int is_negative = 0;
    if (*ptr == '-') {
        is_negative = 1;
        ptr++;
    }
    if (*ptr == '\0') {
        return 0;
    }
    signed long long int result = 0;
    while (*ptr != '\0') {
        if (*ptr < '0' || *ptr > '9') {
            return 0;
        }
        int digit = *ptr - '0';

        if (is_negative) {
            if (-result < (min + digit) / 10) {
                return 0;
            }
        } else {
            if (result > (max - digit) / 10) {
                return 0;
            }
        }
        result = result * 10 + digit;
        ptr++;
    }
    return 1;
}

long long int new_input_metod(int min, int max) {

    int size = 0;
    long long int res = 0;
    int current_pointer = 0;
    int has_num = 0;
    char* line = (char*)malloc(1);
    if (line == NULL) {
        return -2;
    }
    line[0] = '\0';

    while (1) {
        char key = _getch();

        if (key == 27){return -1;}

        if (key == ' ') {

            if (size > 0 && line[size - 1] != ' ' && line[size - 1] != '-') {
                char* temp = realloc(line, size + 2);
                if (temp == NULL) {
                    free(line);
                    return -2;
                }
                line = temp;
                line[size] = ' ';
                line[size + 1] = '\0';
                printf(" ");
                size++;
                has_num = 0;
            }
        }

        else if (key == '-') {

            if (size == 0 || line[size - 1] == ' ') {
                char* temp = realloc(line, size + 2);
                if (temp == NULL) {
                    free(line);
                    return -2;
                }
                line = temp;
                line[size] = '-';
                line[size + 1] = '\0';
                printf("-");
                size++;
                current_pointer = size - 1;
                has_num = 1;
            }
        }

        else if (key >= '0' && key <= '9') {

            if(line[size-1] == '0' && (line[size-2] == ' ' || line[size-2] == '-') || (size == 1 && line[0] == '0'))  { continue;}


            if (has_num) {

                int buf_size = (size - current_pointer) + 2;
                char* buf = (char*)malloc(buf_size);
                if (buf == NULL) {
                    free(line);
                    return -2;
                }
                int iter = 0;
                for (int i = current_pointer; i < size; i++) {
                    buf[iter++] = line[i];
                }
                buf[iter++] = key;
                buf[iter] = '\0';

                if (check_int_1(buf, min, max)) {
                    char* temp = realloc(line, size + 2);
                    if (temp == NULL) {
                        free(buf);
                        free(line);
                        return -2;
                    }
                    line = temp;
                    line[size] = key;
                    line[size + 1] = '\0';
                    printf("%c", key);
                    size++;
                }
                free(buf);
            } else {

                char* temp = realloc(line, size + 2);
                if (temp == NULL) {
                    free(line);
                    return -2;
                }
                line = temp;
                line[size] = key;
                line[size + 1] = '\0';
                printf("%c", key);
                current_pointer = size;
                size++;
                has_num = 1;
            }
        }

        else if (key == '\b') {
            if (size > 0) {
                size--;
                char* temp = realloc(line, size + 1);
                if (temp == NULL) {
                    free(line);
                    return -2;
                }
                line = temp;
                line[size] = '\0';


                printf("\b \b");


                if (size == 0 || line[size - 1] == ' ' || line[size - 1] == '-') {
                    has_num = 0;
                }
            }
        }
        else if (key == '\r' || key == '\n') {
            if(line[size-1] != '-' && size != 0) {
                char *temp = realloc(line, size + 2);
                if (temp == NULL) {
                    free(line);
                    return -2;
                }
                line = temp;
                line[size] = '\0';
                size++;
                break;
            }
        }
    }

    printf("\n");

    sscanf(line, "%lld", &res);

    return res;
}

int input_only_letters(char** last_name_output, int *last_name_output_size){
    char* last_name = (char*)malloc(1);
    int last_name_length = 0;
    do {
        char buf = _getch();
        if (buf == 27){last_name = "ESCAPE_PRESSED_0"; break;}
        if (buf == '\b' || buf == 127) {
            if (last_name_length > 0) {
                last_name_length--;
                char* new_ptr = (char*)realloc(last_name, (last_name_length + 1));
                if (new_ptr != NULL) {
                    last_name = new_ptr;
                }
                printf("\b \b");
            }
        } else if ((buf >= 'à' && buf <= 'ÿ') ||
                   (buf >= 'À' && buf <= 'ß') ||
                   (buf >= 'a' && buf <= 'z') ||
                   (buf >= 'A' && buf <= 'Z')) {
            char* new_ptr = (char*)realloc(last_name, (last_name_length + 2));
            if (new_ptr != NULL) {
                last_name = new_ptr;
                last_name[last_name_length++] = buf;
                last_name[last_name_length] = '\0';
                printf("%c", buf);
            }
                   } else if (buf == '\n' || buf == '\r') {
                       break;
                   }
    } while (1);
    puts("");

    (*last_name_output) = last_name;
    (*last_name_output_size) = last_name_length + 1;
}

int input_only_letters_and_numbers(char* output, int output_size) {
    int length = 0;
    while (1) {
        char buf = _getch();

        if ((buf == '\b' || buf == 127) && length > 0) {
            length--;
            output[length] = '\0';
            printf("\b \b");
        }
        else if (
            ( (buf >= 'à' && buf <= 'ÿ')  ||
              (buf >= 'À' && buf <= 'ß')  ||
              (buf >= 'a' && buf <= 'z')  ||
              (buf >= 'A' && buf <= 'Z')  ||
              (buf >= '0' && buf <= '9') )
            && length < (output_size - 1)
        ) {
            output[length++] = buf;
            output[length] = '\0';
            printf("%c", buf);
        }
        else if (buf == '\n' || buf == '\r') {
            break;
        }
    }
    puts("");
    return length;
}


#endif
