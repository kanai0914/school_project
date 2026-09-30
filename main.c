#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#define RESISTOR_COUNT 3
#define VOLTAGE_COUNT 4
#define VOLTAGE_STEP_COUNT_5 6 // 実験5で使う電圧ステップの数
#define COL_WIDTH 16           // 表の1列の文字幅

// ==========================================
// 汎用関数
// ==========================================

/**
 * @brief 並列の計算をする関数(1/R = 1/R1 + 1/R2 + ... を計算する)
 *
 * @param count 引数にいくつ値を渡すか
 * @param ... double型の値を count 個並べる
 * @return 並列合成した結果
 */
double parallel_calculations(int count, ...);

/**
 * @brief 直列の計算をする関数(R = R1 + R2 + ... を計算する)
 *
 * @param count 引数にいくつ値を渡すか
 * @param ... double型の値を count 個並べる
 * @return 直列合成した結果
 */
double series_calculations(int count, ...);

// buffer clear関数（ループ防止用。read_double_format, read_int_format の内部で使うだけ）
static void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/**
 * @brief 安全にdoubleの値を読む関数(普通にscanfするとバグることがあるので)
 *
 * @param format 表示する文字列。printfと同じ書き方ができる
 * @return 入力されたdouble型の値
 */
static double read_double_format(const char *format, ...);

/**
 * @brief 安全にintの値を読む関数(普通にscanfするとバグることがあるので)
 *
 * @param format 表示する文字列。printfと同じ書き方ができる
 * @return 入力されたint型の値
 */
static int read_int_format(const char *format, ...);

/**
 * @brief 分岐check用関数(入力した値を表示して、直すかどうかを聞く)
 *
 * @param values 確認したい値の配列
 * @param names 値に紐付ける名前
 * @param count 何個分の配列があるのか
 */
static void check_values(double values[], const char *names[], int count);

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
                        int rows, int cols, int precision, double data[rows][cols]);

/**
 * @brief 表の col 列目の平均を計算する(NAN は除く)
 *
 * @details 使用例: double avg = calculate_column_average(4, 3, table, 2);
 *          (4行3列の表 table の、2番目(0始まりなので3列目)の平均を求める)
 */
static double calculate_column_average(int rows, int cols, double data[rows][cols], int col);

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

// ==========================================
// 実験2専用関数
// ==========================================

/**
 * @brief 電圧と電流から 電圧・電流・抵抗 の表データを作る関数(実験2・実験5で共通して使う)
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
 * @brief 一つの抵抗に対し、電圧ごとの電流を入力して値を確認する関数(実験2専用)
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

// ===========
// 実験2
// ===========
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

// ===========
// 実験3
// ===========
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

// 実験4
/**
 * @brief 実験4-1:キルヒホッフの第1法則の検証
 *
 * @details Iin = I1+I2, Iout = I3 として、Ioutを基準に誤差率を計算する。
 *          (2)(3)の4パターン(V1=5,10,15,20V)に加えて、
 *          (4)のR1,R3の電流計を外した再測定も同じ考え方で計算する。
 */
static void experiment4_1(void)
{
    // (2)(3):V1 = 5, 10, 15, 20[V](V2は5[V]で固定)のときのI1, I2, I3を測定
    const char *v1_labels[4] = {"V1=5V", "V1=10V", "V1=15V", "V1=20V"};
    double i1[4], i2[4], i3[4];

    printf("V2=5[V]に固定し、V1を5,10,15,20[V]と変えたときのI1, I2, I3を入力してください\n");
    printf("(矢印の向きに応じて、負の値もそのまま入力すること)\n");
    for (int i = 0; i < 4; i++)
    {
        printf("%s のとき\n", v1_labels[i]);
        i1[i] = read_double_format("I1[mA]>");
        i2[i] = read_double_format("I2[mA]>");
        i3[i] = read_double_format("I3[mA]>");
    }
    check_values(i1, v1_labels, 4);
    check_values(i2, v1_labels, 4);
    check_values(i3, v1_labels, 4);

    // 表4.2 (2)(3):Iin, Iout, Iin-Iout, 誤差率
    double table_23[4][4];
    for (int i = 0; i < 4; i++)
    {
        double iin = i1[i] + i2[i];
        double iout = i3[i];
        table_23[i][0] = iin;
        table_23[i][1] = iout;
        table_23[i][2] = iin - iout;
        table_23[i][3] = calculate_error_rate(iin, iout); // Iout を基準にする
    }
    const char *headers_42[4] = {"Iin[mA]", "Iout[mA]", "Iin-Iout[mA]", "誤差率[%]"};
    print_table("表4.2 (2)(3)", headers_42, v1_labels, 4, 4, 5, table_23);
    printf("\n");

    // 表4.2 (4):R1とR3の電流計を外してI2を測り直す(I1, I3はV1=5Vのときの値を使う)
    printf("R1とR3の電流計を外して、I2を測り直してください\n");
    double i2_retest = read_double_format("I2[mA]>");
    double iin_4 = i1[0] + i2_retest;
    double iout_4 = i3[0];
    double table_4[1][4] = {{iin_4, iout_4, iin_4 - iout_4, calculate_error_rate(iin_4, iout_4)}};
    const char *row_label_4[1] = {"(4) I2再測定"};
    print_table("表4.2 (4)", headers_42, row_label_4, 1, 4, 5, table_4);
}

/**
 * @brief 実験4-2:キルヒホッフの第2法則の検証
 *
 * @details 3つのループ(V1,R1,R2)(V2,R2,R3)(V1,V2,R1,R3)について、
 *          起電力の総和Eを基準に、電圧降下の総和VRとの誤差率を計算する。
 */
static void experiment4_2(void)
{
    printf("V1, V2, Vr1, Vr2, Vr3を入力してください\n");
    printf("(矢印の向きに応じて、負の値もそのまま入力すること)\n");
    const char *names[5] = {"V1", "V2", "Vr1", "Vr2", "Vr3"};
    double values[5];
    for (int i = 0; i < 5; i++)
    {
        values[i] = read_double_format("%s[V]>", names[i]);
    }
    check_values(values, names, 5);
    double v1 = values[0], v2 = values[1];
    double vr1 = values[2], vr2 = values[3], vr3 = values[4];

    double table[3][4];
    double e, vr;

    e = v1;
    vr = series_calculations(2, vr1, vr2);
    table[0][0] = e;
    table[0][1] = vr;
    table[0][2] = vr - e;
    table[0][3] = calculate_error_rate(vr, e);

    e = v2;
    vr = series_calculations(2, vr2, vr3);
    table[1][0] = e;
    table[1][1] = vr;
    table[1][2] = vr - e;
    table[1][3] = calculate_error_rate(vr, e);

    e = series_calculations(2, v1, v2);
    vr = series_calculations(2, vr1, vr3);
    table[2][0] = e;
    table[2][1] = vr;
    table[2][2] = vr - e;
    table[2][3] = calculate_error_rate(vr, e);

    const char *headers[4] = {"E[V]", "VR[V]", "VR-E[V]", "誤差率[%]"};
    const char *row_labels[3] = {"V1,R1,R2", "V2,R2,R3", "V1,V2,R1,R3"};
    print_table("表4.4", headers, row_labels, 3, 4, 5, table);
}

// ==========================================
// 実験5専用関数
// ==========================================

/**
 * @brief 1つの抵抗・1つの回路について、電圧ごとの電流を入力してV/Iの表を作り、表示する
 *
 * @param resistor_name 抵抗の名前(表示用。例:"0.5Ω")
 * @param circuit_name 回路の名前(表示用。例:"(a)")
 * @param voltages 各行で設定する電圧の値
 * @param count 行数(電圧ステップの数)
 * @return V/I の平均値(NANを除いた平均。calculate_column_averageで計算)
 */
static double measure_and_average_v_over_i(const char *resistor_name, const char *circuit_name,
                                           const double voltages[], int count)
{
    double currents[VOLTAGE_STEP_COUNT_5];
    char v_label_storage[VOLTAGE_STEP_COUNT_5][16];
    const char *v_labels[VOLTAGE_STEP_COUNT_5];

    printf("【%s】実験回路%s\n", resistor_name, circuit_name);
    for (int i = 0; i < count; i++)
    {
        snprintf(v_label_storage[i], sizeof(v_label_storage[i]), "%.1fV", voltages[i]);
        v_labels[i] = v_label_storage[i];
        currents[i] = read_double_format("%sのときの電流[A]>", v_labels[i]);
    }
    check_values(currents, v_labels, count);

    double table[VOLTAGE_STEP_COUNT_5][3];
    make_iv_table(voltages, currents, count, table);

    const char *headers[3] = {"V[V]", "I[A]", "V/I[Ω]"};
    print_table(resistor_name, headers, NULL, count, 3, 5, table);
    printf("\n");

    return calculate_column_average(count, 3, table, 2);
}

// ===========
// 実験5(表5.4は作らず、表5.2と表5.5のみ)
// ===========
static void experiment5(void)
{
    const char *resistor_names[3] = {"0.5Ω", "560Ω", "150kΩ"};
    double nominal[3] = {0.5, 560.0, 150000.0}; // 公称値(Ω単位に統一)

    double voltages_05[VOLTAGE_STEP_COUNT_5] = {0.5, 0.4, 0.3, 0.2, 0.1, 0.0};
    double voltages_560[VOLTAGE_STEP_COUNT_5] = {10.0, 8.0, 6.0, 4.0, 2.0, 0.0};
    double voltages_150k[VOLTAGE_STEP_COUNT_5] = {25.0, 20.0, 15.0, 10.0, 5.0, 0.0};
    const double *voltages[3] = {voltages_05, voltages_560, voltages_150k};

    double error_rate[3][2]; // 行=抵抗、列=回路(a),(b)

    for (int i = 0; i < 3; i++)
    {
        double avg_a = measure_and_average_v_over_i(resistor_names[i], "(a)", voltages[i], VOLTAGE_STEP_COUNT_5);
        double avg_b = measure_and_average_v_over_i(resistor_names[i], "(b)", voltages[i], VOLTAGE_STEP_COUNT_5);

        error_rate[i][0] = calculate_error_rate(avg_a, nominal[i]);
        error_rate[i][1] = calculate_error_rate(avg_b, nominal[i]);
    }

    // 表5.5:公称値との誤差率
    const char *headers_55[2] = {"回路(a)[％]", "回路(b)[％]"};
    print_table("表5.5 公称値との誤差率", headers_55, resistor_names, 3, 2, 5, error_rate);
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
        experiment4_1();
        experiment4_2();
        break;
    case 5:
        experiment5();
        break;
    default:
        printf("2から5までの番号を入力してください\n");
        break;
    }
    return 0;
}

/*
=============================================================
//関数の中の定義 ※見なくていいです
=============================================================
 */

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