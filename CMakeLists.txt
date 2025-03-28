cmake_minimum_required(VERSION 3.30)
project(sem_2_laba_3 C)

set(CMAKE_C_STANDARD 11)

add_executable(sem_2_laba_3 main.c
        INTERFACE.h
        Header.h
        byte_field_task.h
        Error_codes.h
        BANK_SYSTEM_TASK.h
        MAIN_DATABASE_FUNCTIONS.h
)
