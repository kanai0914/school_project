#include <stdio.h>

int task_num;


typedef enum  {
    DIAGRAM3_1,
    DIAGRAM3_2,
    DIAGRAM3_3,
    DIAGRAM4_1,
    DIAGRAM4_2,
    DIAGRAM5_a,
    DIAGRAM5_b,
}DIAGRAM; // this is type name

void view_electric_circuit(DIAGRAM diagram){

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