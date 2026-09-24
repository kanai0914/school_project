#include <stdio.h>
#include <stdarg.h>

// 並列の計算
double parallel_calculations(int count, ...)
{
    if (count <= 0)
    {
        return 0.0;
    }
    va_list args;
    va_start(args, count);
    double parallel_sum = 0.0;
    for (int i = 0; i < count; i++)
    {
        double reciprocal = 1 / va_arg(args, double);
        parallel_sum += reciprocal;
    }
    double result = 1 / parallel_sum;
    return result;
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
    double r1, r2, r3;
    double result;
    int count;
    printf("please enter number>");
    scanf("%lf %lf %lf", &r1, &r2, &r3);
    result = parallel_calculations(3, r1, r2, r3);
    printf("%.5lf\n", result);
    return 0;
}