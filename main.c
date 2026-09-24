#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define RESISTOR_COUNT 3
#define VOLTAGE_COUNT 4

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
static int read_int_format(const char *format, ...)
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

// 分岐check用関数
static void check_values(double values[], const char *names[], int count)
{
    while (1)
    {
        printf("are you ok this value?\n");
        for (int i = 0; i < count; i++)
        {
            printf("%s>%lf\n", names[i], values[i]);
        }
        printf("if YES. please enter 1.\n if NO. please enter 0.\n");
        int decision = read_int_format("enter number>");

        if (decision == 1)
        {
            return;
        }
        else if (decision == 0)
        {
            printf("please select change value\n");
            for (int i = 0; i < count; i++)
            {
                printf("%d:%s  ", i + 1, names[i]);
            }
            printf("%d:All\n", count + 1);
            int select_number = read_int_format("enter number>");

            if (select_number == count + 1) // 全部変える
            {
                for (int i = 0; i < count; i++)
                {
                    values[i] = read_double_format("%s>", names[i]);
                }
            }
            else if (select_number >= 1 && select_number <= count) // 1個だけ変える
            {
                values[select_number - 1] = read_double_format("%s>", names[select_number - 1]);
            }
            else
            {
                printf("please enter a number from 1 to %d.\n", count + 1);
            }
        }
        else
        {
            printf("Please enter a value 0 or 1\n");
        }
    }
}

// 1つの抵抗について、電圧ごとの電流を入力して確認する
static void measure_currents(const char *resistor_name, const char *voltages[],
                             double currents[], int count)
{
    printf("When %s\n", resistor_name);
    for (int i = 0; i < count; i++)
    {
        currents[i] = read_double_format("please enter current when voltage %s>", voltages[i]);
    }
    check_values(currents, voltages, count);
}

// 実験2:全部の抵抗について電流を測って表示する
static void experiment2(const char *resistor_names[])
{
    const char *voltages[VOLTAGE_COUNT] = {"2V", "4V", "6V", "8V"};
    double currents[RESISTOR_COUNT][VOLTAGE_COUNT];

    printf("Please enter the information required to create Table 2.1.\n");
    for (int i = 0; i < RESISTOR_COUNT; i++)
    {
        measure_currents(resistor_names[i], voltages, currents[i], VOLTAGE_COUNT);
    }

    // 結果の表示
    for (int i = 0; i < RESISTOR_COUNT; i++)
    {
        printf("[%s]\n", resistor_names[i]);
        for (int j = 0; j < VOLTAGE_COUNT; j++)
        {
            printf("  %s:%lf\n", voltages[j], currents[i][j]);
        }
    }
}

int main(void)
{
    double r[RESISTOR_COUNT];
    const char *names[RESISTOR_COUNT] = {"R1", "R2", "R3"};

    printf("please enter three resistance value\n");
    for (int i = 0; i < RESISTOR_COUNT; i++)
    {
        r[i] = read_double_format("%s:", names[i]);
    }

    check_values(r, names, RESISTOR_COUNT);

    for (int i = 0; i < RESISTOR_COUNT; i++)
    {
        printf("%s:%.5lf\n", names[i], r[i]);
    }

    printf("please select experiment number.\n");
    int experiment_number = read_int_format("range is 2 ~ 6>");
    switch (experiment_number)
    {
    case 2:
        experiment2(names);
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
        printf("Please enter a value 2 or 6\n");
        break;
    }
    return 0;
}