#include <stdio.h>

/*
 * 统计成绩高于平均分的学生人数，同时通过指针带出平均分
 * score: 成绩数组
 * n:     实际输入的学生人数
 * pAvg:  用于输出平均分的指针（*pAvg 会被填充）
 * 返回:  高于平均分的人数
 */
int countAboveAverage(double score[], int n, double *pAvg)
{
    int i;
    int cnt = 0;
    double sum = 0.0;

    if (n <= 0) {
        if (pAvg) *pAvg = 0.0;
        return 0;
    }

    /* 第一步：求总分 */
    for (i = 0; i < n; i++) {
        sum += score[i];
    }

    /* 第二步：算平均分 */
    *pAvg = sum / n;

    /* 第三步：统计高于平均分的人数 */
    for (i = 0; i < n; i++) {
        if (score[i] > *pAvg) {
            cnt++;
        }
    }

    return cnt;
}

int main(void)
{
    int i;
    int n = 0;              /* 实际输入的学生人数 */
    double scores[40];
    double x;
    double avg;

    printf("Enter scores (negative number to end, up to 40 students):\n");

    for (i = 0; i < 40; i++) {
        printf("Student %d: ", i + 1);
        if (scanf("%lf", &x) != 1) {   /* %lf 读取 double，且检查返回值 */
            printf("Invalid input. Stop.\n");
            break;
        }

        if (x < 0) {                   /* 负数结束输入 */
            break;
        }

        scores[n] = x;                 /* 只保存有效成绩 */
        n++;
    }

    if (n == 0) {
        printf("No scores entered.\n");
        return 0;
    }

    int above = countAboveAverage(scores, n, &avg);

    printf("\nTotal students: %d\n", n);
    printf("Average score: %.2f\n", avg);
    printf("Students above average: %d\n", above);

    return 0;
}
