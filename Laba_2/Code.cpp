#include <iostream>
#include <omp.h>
#include <locale.h>
#include <vector>

using namespace std;

const int N = 700;

int main()
{
    setlocale(LC_ALL, "Russian");

    vector<vector<double>> A(N, vector<double>(N, 1.0));
    vector<vector<double>> B(N, vector<double>(N, 2.0));
    vector<vector<double>> C(N, vector<double>(N, 0.0));

    double start, finish;

    // Последовательное умножение
    start = omp_get_wtime();

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    finish = omp_get_wtime();
    cout << "Последовательное время: "
        << finish - start << " сек.\n";

    // Параллельное умножение
    for (int threads : {2, 4, 8}) {
        fill(C.begin(), C.end(), vector<double>(N, 0.0));

        omp_set_num_threads(threads);
        start = omp_get_wtime();

#pragma omp parallel for
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                for (int k = 0; k < N; k++)
                {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }

        finish = omp_get_wtime();

        cout << "Потоков: " << threads
            << ", время: " << finish - start
            << " сек.\n";
    }

    return 0;
}
