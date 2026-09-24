#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#define RESISTOR_COUNT 3
#define VOLTAGE_COUNT 4
#define COL_WIDTH 16 // 表の1列の文字幅

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

/**
 * @brief 表を表示させる汎用関数
 *
 * @param title 表のタイトル
 * @param headers 各列の見出し
 * @param rows 表の行数
 * @param cols 列数
 * @param precision 小数点以下の桁数
 * @param data rows × cols の表データ
 * memo: width pointa
 */
static void print_table(const char *title, const char *headers[], int rows, int cols,
                        int precision, double data[rows][cols])
{
    printf("[%s]\n", title);

    for (int j = 0; j < cols; j++)
    {
        printf("%-*s", COL_WIDTH, headers[j]);
    }
    printf("\n");

    for (int j = 0; j < cols * COL_WIDTH; j++)
    {
        putchar('-');
    }
    printf("\n");

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (isnan(data[i][j]))
            {
                printf("%-*s", COL_WIDTH, "---");
            }
            else
            {
                printf("%-*.*f", COL_WIDTH, precision, data[i][j]);
            }
        }
        printf("\n");
    }
}

// 電圧と電流から「電圧・電流・抵抗(R = V / I)」の表データを作る
static void make_iv_table(const double voltages[], const double currents[],
                          int count, double table[][3])
{
    for (int i = 0; i < count; i++)
    {
        table[i][0] = voltages[i];
        table[i][1] = currents[i];
        table[i][2] = (currents[i] > 0.0) ? voltages[i] / currents[i] : NAN;
    }
}

// 実験2:全部の抵抗について電流を測って表示する
static void experiment2(const char *resistor_names[], int resistor_count)
{
    // 表示・確認用の名前と、計算用の数値は、同じ順番で揃えておく
    const char *voltage_names[VOLTAGE_COUNT] = {"2V", "4V", "6V", "8V"};
    const double voltage_values[VOLTAGE_COUNT] = {2.0, 4.0, 6.0, 8.0};
    const char *headers[3] = {"Voltage[V]", "Current[A]", "Resistance[ohm]"};
    double currents[RESISTOR_COUNT][VOLTAGE_COUNT];
    double table[VOLTAGE_COUNT][3];

    printf("Please enter the information required to create Table 2.1.\n");
    for (int i = 0; i < resistor_count; i++)
    {
        measure_currents(resistor_names[i], voltage_names, currents[i], VOLTAGE_COUNT);
    }

    for (int i = 0; i < resistor_count; i++)
    {
        make_iv_table(voltage_values, currents[i], VOLTAGE_COUNT, table);
        print_table(resistor_names[i], headers, VOLTAGE_COUNT, 3, 5, table);
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
        printf("%s:%lf\n", names[i], r[i]);
    }

    printf("please select experiment number.\n");
    int experiment_number = read_int_format("range is 2 ~ 6>");
    switch (experiment_number)
    {
    case 2:
        experiment2(names, RESISTOR_COUNT);
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