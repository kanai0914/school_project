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
        printf("有効な数字を入力してください.\n");
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
        printf("有効な数字を入力してください.\n");
        clear_input_buffer();
    }
}

/**
 * @brief 分岐check用関数
 *
 * @param values 確認したい値の配列
 * @param names 値に紐付ける名前
 * @param count 何個分の配列があるのか。
 *
 * @details valuesとnamesの配列の数は一緒にすること！！
 */
static void check_values(double values[], const char *names[], int count)
{
    while (1)
    {
        printf("この値で大丈夫ですか？\n");
        for (int i = 0; i < count; i++)
        {
            printf("%s>%lf\n", names[i], values[i]);
        }
        printf("もし良ければ 1 を入力してください。\nもしだめなら 0 を入力してください。\n");
        int decision = read_int_format("入力 >");

        if (decision == 1)
        {
            return;
        }
        else if (decision == 0)
        {
            printf("変えたい値を選択してください\n");
            for (int i = 0; i < count; i++)
            {
                printf("%d:%s  ", i + 1, names[i]);
            }
            printf("%d:すべて\n", count + 1);
            int select_number = read_int_format("入力 >");

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
                printf("1 から %dの番号を入力してください.\n", count + 1);
            }
        }
        else
        {
            printf("0 か 1 を入力してください\n");
        }
    }
}

/**
 * @brief 一つの抵抗に対し、電圧ごとの電流を入力して値を確認する関数
 *
 * @param resistor_name 抵抗の名前
 * @param voltages 何ボルトの電圧がかけているか
 * @param currents 何アンペアの電流がかかっているか
 * @param count 何個分の配列があるのか
 *
 * @details voltagesとcurrentsの配列の数は一緒にすること！！
 */
static void measure_currents(const char *resistor_name, const char *voltages[],
                             double currents[], int count)
{
    printf("%sの時\n", resistor_name);
    for (int i = 0; i < count; i++)
    {
        currents[i] = read_double_format("%sのときの電流 >", voltages[i]);
    }
    check_values(currents, voltages, count);
}

/**
 * @brief 表を表示させる汎用関数
 *
 * @param title 表のタイトル(row_labels が NULL のときは [ ] で囲んで表示、
 *              NULL でないときは左上の角に置く)
 * @param headers 各列の見出し
 * @param row_labels 各行の先頭に付けるラベル。行のラベルが不要なら NULL を渡す
 * @param rows 表の行数
 * @param cols 列数
 * @param precision 小数点以下の桁数
 * @param data rows × cols の表データ
 */
static void print_table(const char *title, const char *headers[], const char *row_labels[],
                        int rows, int cols, int precision, double data[rows][cols])
{
    // ヘッダー行
    if (row_labels == NULL)
    {
        printf("[%s]\n", title);
    }
    else
    {
        printf("%-*s", COL_WIDTH, title); // 左上の角に title を置く
    }
    for (int j = 0; j < cols; j++)
    {
        printf("%-*s", COL_WIDTH, headers[j]);
    }
    printf("\n");

    // 区切り線(ラベル列があるぶん、幅を1列分伸ばす)
    int line_cols = (row_labels == NULL) ? cols : cols + 1;
    for (int j = 0; j < line_cols * COL_WIDTH; j++)
    {
        putchar('-');
    }
    printf("\n");

    // データ行
    for (int i = 0; i < rows; i++)
    {
        if (row_labels != NULL)
        {
            printf("%-*s", COL_WIDTH, row_labels[i]);
        }
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

/**
 * @brief 電圧と電流から 電圧・電流・抵抗 の表データを作る関数(実験2専用)
 *
 * @param voltages 電圧
 * @param currents 電流
 * @param count 何個分の配列があるのか
 * @param table 表データの格納先(count 行 × 3列)
 */
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

/**
 * @brief 表の col 列目の平均を計算する(NAN は除く)
 */
static double calculate_column_average(int rows, int cols, double data[rows][cols], int col)
{
    double sum = 0.0;
    int valid_count = 0;

    for (int i = 0; i < rows; i++)
    {
        if (!isnan(data[i][col]))
        {
            sum += data[i][col];
            valid_count++;
        }
    }

    if (valid_count == 0) // 全部 NAN だった場合、0 割りを避ける
    {
        return NAN;
    }
    return sum / valid_count;
}

/**
 * @brief 誤差率(%)を計算する
 *
 * @param measured 測定値・比較したい値
 * @param reference 基準にする値(真値とみなす方)
 */
static double calculate_error_rate(double measured, double reference)
{
    return (measured - reference) / reference * 100.0;
}

// 実験2:全部の抵抗について電流を測って表示する
static void experiment2(const char *resistor_names[], int resistor_count, const double nominal_value[])
{
    // 表示・確認用の名前と、計算用の数値は、同じ順番で揃えておく
    const char *voltage_names[VOLTAGE_COUNT] = {"2V", "4V", "6V", "8V"};
    const double voltage_values[VOLTAGE_COUNT] = {2.0, 4.0, 6.0, 8.0};
    const char *headers[3] = {"電圧[V]", "電流[A]", "抵抗値[kΩ]"};
    double currents[RESISTOR_COUNT][VOLTAGE_COUNT];
    double table[VOLTAGE_COUNT][3];
    double avg_resistance[1][RESISTOR_COUNT]; // 1行 × 3列(R1,R2,R3の平均をこの1行に並べる)
    double error_resistance[1][RESISTOR_COUNT];

    printf("表2.1を作るために必要な項目を入力してください。\n");
    for (int i = 0; i < resistor_count; i++)
    {
        measure_currents(resistor_names[i], voltage_names, currents[i], VOLTAGE_COUNT);
    }

    // 表2.1の作成 memo:tableがfor文ごとに更新される
    for (int i = 0; i < resistor_count; i++)
    {
        make_iv_table(voltage_values, currents[i], VOLTAGE_COUNT, table);
        print_table(resistor_names[i], headers, NULL, VOLTAGE_COUNT, 3, 5, table);
        printf("\n");
        avg_resistance[0][i] = calculate_column_average(VOLTAGE_COUNT, 3, table, 2);
    }

    // 表2.2の作成(コンダクタンスと抵抗値は入れていない)
    const char *headers_2_3[RESISTOR_COUNT] = {"[kΩ]", "[kΩ]", "[kΩ]"};
    const char *avg_row_labels[1] = {"Rの平均値"};
    print_table("抵抗の公称値", headers_2_3, avg_row_labels, 1, RESISTOR_COUNT, 5, avg_resistance);
    printf("\n");

    // 誤差率の計算(基準は公称値 nominal_value[i])
    for (int i = 0; i < resistor_count; i++)
    {
        error_resistance[0][i] = calculate_error_rate(avg_resistance[0][i], nominal_value[i]);
    }

    // 表2.3の作成
    const char *error_row_labels[1] = {"誤差率 Ea[％]"};
    print_table("抵抗の公称値", headers_2_3, error_row_labels, 1, RESISTOR_COUNT, 5, error_resistance);
}

// 実験3
static void experiment3(void)
{
    // 実験2で求めたRaの入力に使う配列設定
    const char *names[RESISTOR_COUNT] = {"Ra1", "Ra2", "Ra3"};
    double ra[RESISTOR_COUNT];

    // 実験2で求めたRaの入力
    printf("実験2で求めたそれぞれのRaの値(それぞれの抵抗の平均値)を入力してください\n");
    for (int i = 0; i < RESISTOR_COUNT; i++)
    {
        ra[i] = read_double_format("%s:", names[i]);
    }
    check_values(ra, names, RESISTOR_COUNT);

    // 表3.1で使う配列の設定
    const char *row_labels_1[6] = {"V[V]", "V1[V]", "V2[V]", "V3[V]", "I[mA]", "Rs{kΩ}"};
    const char *header[1] = {"理論値"};
    double table[7][1];

    // 表3.1の値計算
    table[0][0] = 10.0;                                                     // V
    table[5][0] = series_calculations(RESISTOR_COUNT, ra[0], ra[1], ra[2]); // Rs
    for (int i = 0; i < RESISTOR_COUNT; i++)
    {
        table[i + 1][0] = table[0][0] * ra[i] / table[5][0]; // Vi = V × Rai / Rs
    }
    table[4][0] = table[0][0] / table[5][0]; // I = V / Rs

    // 表3.1の表示
    print_table("表3.1", header, row_labels_1, 6, 1, 5, table);

    // 表3.2で使う配列の設定
    const char *row_labels_2[6] = {"V[V]", "I1[mA]", "I2[mA]", "I3[mA]", "I[mA]", "Rp[kohm]"};

    // 表3.2の値計算
    table[5][0] = parallel_calculations(RESISTOR_COUNT, ra[0], ra[1], ra[2]); // Rp
    table[4][0] = table[0][0] / table[5][0];                                  // I
    for (int i = 0; i < RESISTOR_COUNT; i++)
    {
        table[i + 1][0] = (1 / ra[i]) * table[4][0] * table[5][0]; // Ii = 1/Rai * I * Rp
    }
    // 表3.2の表示
    print_table("表3.2", header, row_labels_2, 6, 1, 5, table);

    // 表3.3で使う配列と変数の設定
    const char *row_labels_3[7] = {"V[V]", "V1[V]", "V2[V]", "I[mA]", "I2[mA]", "I3[mA]", "Rsp[kohm]"};
    double parallel_resistor_sum = parallel_calculations(2, ra[1], ra[2]); // R2‖R3

    // 表3.3の値の計
    table[6][0] = series_calculations(2, parallel_resistor_sum, ra[0]); // Rsp = R1 + R2‖R3
    table[1][0] = table[0][0] * ra[0] / table[6][0];                    // V1 = V×R1/Rsp
    table[2][0] = table[0][0] * parallel_resistor_sum / table[6][0];    // V2 = V×(R2‖R3)/Rsp
    table[3][0] = table[0][0] / table[6][0];                            // I  = V/Rsp
    table[4][0] = table[3][0] * parallel_resistor_sum / ra[1];          // I2 = I×(R2‖R3)/Ra2
    table[5][0] = table[3][0] * parallel_resistor_sum / ra[2];          // I3 = I×(R2‖R3)/Ra3

    // 表3.3の表示
    print_table("表3.3", header, row_labels_3, 7, 1, 5, table);
}

int main(void)
{
    double r[RESISTOR_COUNT];
    const char *names[RESISTOR_COUNT] = {"R1", "R2", "R3"};

    printf("R1 ~ R3の値を入力してください\n");
    for (int i = 0; i < RESISTOR_COUNT; i++)
    {
        r[i] = read_double_format("%s[kΩ]:", names[i]);
    }

    check_values(r, names, RESISTOR_COUNT);

    for (int i = 0; i < RESISTOR_COUNT; i++)
    {
        printf("%s:%lf\n", names[i], r[i]);
    }

    printf("実験の番号を入力してください。\n");
    int experiment_number = read_int_format("実験番号 2 ~ 6 >");
    switch (experiment_number)
    {
    case 2:
        experiment2(names, RESISTOR_COUNT, r);
        break;
    case 3:
        experiment3();
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    default:
        printf("2から6までの番号を入力してください\n");
        break;
    }
    return 0;
}