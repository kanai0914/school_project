#include <stdio.h>

int task_num;

int select_task(int task)
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
        break;

    default:
        printf("sorry\n");
        break;
    }
}

int main(void)
{
    // neko
    printf("please enter a number >>");
    pritnf("1. task1\n");
    scanf("%d", &task_num);
    select_task(task_num);
    return 0;
}