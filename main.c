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
    double reciprocal_sum = 0.0;
    for (int i = 0; i < count; i++)
    {
        double reciprocal = 1 / va_arg(args, double);
        reciprocal_sum += reciprocal;
    }
    double parallel_sum = 1 / reciprocal_sum;
    return parallel_sum;
}

double series_calculations(int count, ...)
{
    if (count <= 0)
    {
        return 0.0;
    }
    va_list args;
    va_start(args, count);
    double series_sum = 0.0;
    for (int i = 0; i < count; i++)
    {
        series_sum += va_arg(args, double);
    }
    return series_sum;
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
    int count;
    // 並列処理確認プログラム
    printf("please enter number>");
    scanf("%lf %lf %lf", &r1, &r2, &r3);
    double result_parallel = parallel_calculations(3, r1, r2, r3);
    printf("%.5lf\n", result_parallel);

    // 直列処理確認プログラム
    printf("please enter number>");
    scanf("%lf %lf %lf", &r1, &r2, &r3);
    double result_series = series_calculations(3, r1, r2, r3);
    printf("%.5lf\n", result_series);

    return 0;
}