#include <stdio.h>

#ifndef _WIN32

#endif

#ifdef _WIN32

#endif

int task_num;

typedef enum
{
    DIAGRAM3_1,
    DIAGRAM3_2,
    DIAGRAM3_3,
    DIAGRAM4_1,
    DIAGRAM4_2,
    DIAGRAM5_a,
    DIAGRAM5_b,
} DIAGRAM; // this is type name

void view_electric_circuit(DIAGRAM diagram)
{
    switch (diagram)
    {
    case DIAGRAM3_1:
        // 実験３の回路1を表示
        break;

    case DIAGRAM3_2:
        // 実験３の回路2を表示
        break;
    case DIAGRAM3_3:
        // 実験３の回路3を表示
        break;
    case DIAGRAM4_1:
        // 実験4-1の回路を表示
        break;
    case DIAGRAM4_2:
        // 実験4-2の回路を表示
        break;
    case DIAGRAM5_a:
        // 実験5の回路aを表示
        break;
    case DIAGRAM5_b:
        // 実験5の回路を表示
        break;
    }
}

void select_task(int task)
{
    switch (task)
    {
    case 1:
        printf("1\n");
        break;

    case 2:
        printf("2\n");
        break;

    case 3:
        printf("3\n");
        break;

    case 4:
        printf("4\n");
        printf("nekoneko\n");

        break;

    default:
        printf("sorry\n");
        break;
    }
}

int main(void)
{

    printf("please enter a number >>");
    pritntf("1. task1\n");
    scanf("%d", &task_num);
    select_task(task_num);
    return 0;
}