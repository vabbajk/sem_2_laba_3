cmake_minimum_required(VERSION 3.30)
project(sem_2_laba_3 C)

set(CMAKE_C_STANDARD 11)

add_executable(sem_2_laba_3 main.c
        functions_to_interface.h
        Header.h
        task_1.h
        Error_codes.h
        task_2.h
        functions_to_task_2.h
)
