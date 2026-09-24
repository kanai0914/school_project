#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

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
    va_end(args);
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
    va_end(args);
    return series_sum;
}

int main(void)
{
    double r1, r2, r3;

    // 抵抗の入力
    printf("please enter three resistance value\n");
    printf("R1:");
    scanf("%lf", &r1);
    printf("R2:");
    scanf("%lf", &r2);
    printf("R3:");
    scanf("%lf", &r3);

    int decision = 0;

    while (1)
    {
        printf("are you ok this value?\n R1>%lf\nR2>%lf\nR3>%lf\n", r1, r2, r3);
        printf("if YES. please enter 1.\n if NO. please enter 0.\n [y:n]>");
        scanf("%d", &decision);

        if (decision == 1)
        {
            printf("R1:%lf\nR2:%lf\nR3:%lf\n", r1, r2, r3);
            break;
        }
        else if (decision == 0)
        {
            int select_number = 0;
            printf("please select change resistance value\n");
            printf("enter_number:select_value\n");
            printf("1:R1  2:R2  3:R3 4:All resistance\n  enter number>");
            scanf("%d", &select_number);
            switch (select_number)
            {
            case 1:
                printf("R1>");
                scanf("%lf", &r1);
                break;
            case 2:
                printf("R2>");
                scanf("%lf", &r2);
                break;
            case 3:
                printf("R3>");
                scanf("%lf", &r3);
                break;
            case 4:
                printf("All resistance>");
                printf("R1>");
                scanf("%lf", &r1);
                printf("R2>");
                scanf("%lf", &r2);
                printf("R3>");
                scanf("%lf", &r3);
                break;
            default:
                printf("the input was outside the valid range, so returning to the beginning\n");
                break;
            }
        }
        else
        {
            printf("the input was outside the valid range, so returning to the beginning\n");
        }
    }

    /*
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
*/
    return 0;
}