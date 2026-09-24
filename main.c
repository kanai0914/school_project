#include <stdio.h>
#include <stdlib.h>
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
    va_end(args);
    double parallel_sum = 1 / reciprocal_sum;
    return parallel_sum;
}

// 直列の計算
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

// buffer clear関数（ループ防止用）
static void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// 安全にdoubleの値を読む関数
static double read_double_format(const char *format, ...)
{
    double value;
    while (1)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);

        int result = scanf("%lf", &value);

        if (result == 1)
        {
            clear_input_buffer();
            return value;
        }
        else if (result == EOF)
        {
            printf("\ninput closed. exiting.\n");
            exit(1);
        }
        printf("please enter a valid value.\n");
        clear_input_buffer();
    }
}

// 安全にintの値を読む関数
static int read_int(const char *format, ...)
{
    int value;
    while (1)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);

        int result = scanf("%d", &value);

        if (result == 1)
        {
            clear_input_buffer();
            return value;
        }
        else if (result == EOF)
        {
            printf("\ninput closed. exiting.\n");
            exit(1);
        }
        printf("please enter a valid value.\n");
        clear_input_buffer();
    }
}

void check_value(double array[], int count, ...)
{

    va_list args;
    va_start(args, count);
    char sentence[count];
    for (int i = 0; i < count; i++)
    {
        sentence[i] = va_arg(args, char);
    }
    va_end(args);

    printf("Are you ok this value?\n");
    for (int i = 0; i < count; i++)
    {
        printf("%s>%d\n", sentence[i], array[i]);
    }
    printf("if YES. please enter 1.\n if NO. please enter 0.\n");
    int decision = read_int("enter number>");

    if (decision == 1)
    {
        for (int i = 0; i < count; i++)
        {
            printf("%s>%d\n", sentence[i], array[i]);
        }
    }
    else if (decision == 0)
    {
        printf("please select change resistance value\n");
        printf("enter_number:select_value\n");
        for (int i = 0; i < count; i++)
        {
            printf("%d:%s ", i + 1, sentence[i]);
        }
        printf("%d:all change\n", count + 1);
        int select_number = read_int("enter number>");

        for (int i = 0; i < count; i++)
        {
            if (select_number = count + 1)
            {
                for (int j = 0; j < count; j++)
                {
                    array[j] = read_double_format("enter%s", sentence[j]);
                }
                break;
            }
            if (select_number = i + 1)
            {
                array[i] = read_double_format("enter%s", sentence[i]);
                break;
            }
        }
    }
    else
    {
        printf("Please enter a value 0 or 1");
    }
}

int main(void)
{
    double r1, r2, r3;
    double r[3];

    // 抵抗の入力
    printf("please enter three resistance value\n");
    r1 = read_double_format("R1:");
    r2 = read_double_format("R2:");
    r3 = read_double_format("R3:");

    // 入力した抵抗が本当に大丈夫なのか確認
    while (1)
    {
        printf("are you ok this value?\n R1>%lf\nR2>%lf\nR3>%lf\n", r1, r2, r3);
        printf("if YES. please enter 1.\n if NO. please enter 0.\n");
        int decision = read_int("enter number>");

        if (decision == 1)
        {
            printf("R1:%lf\nR2:%lf\nR3:%lf\n", r1, r2, r3);
            break;
        }
        else if (decision == 0)
        {
            printf("please select change resistance value\n");
            printf("enter_number:select_value\n");
            printf("1:R1  2:R2  3:R3 4:All resistance\n");
            int select_number = read_int("enter number>");

            switch (select_number)
            {
            case 1:
                r1 = read_double_format("R1>");
                break;
            case 2:
                r2 = read_double_format("R2>");
                break;
            case 3:
                r3 = read_double_format("R3>");
                break;
            case 4:
                r1 = read_double_format("R1>");
                r2 = read_double_format("R2>");
                r3 = read_double_format("R3>");
                break;
            default:
                printf("Please enter a value between 1 and 4.");
            }
        }
        else
        {
            printf("Please enter a value 0 or 1");
        }
    }

    printf("please select experiment number.\n");
    int experiment_number = read_int("range is 2 ~ 6>");
    switch (experiment_number)
    {
    case 2:
        printf("Please enter the information required to create Table 2.1.\n");
        printf("first,please enter r1 current value");
        double a2, a4, a6, a8;
        a2 = read_double_format("voltage>2 current>");
        a4 = read_double_format("voltage>4 current>");
        a6 = read_double_format("voltage>6 current>");
        a8 = read_double_format("voltage>8 current>");

        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    default:
        break;
    }

    /*
    // 並列・直列の確認
    printf("parallel: %.5lf\n", parallel_calculations(3, r1, r2, r3));
    printf("series  : %.5lf\n", series_calculations(3, r1, r2, r3));
    */

    return 0;
}