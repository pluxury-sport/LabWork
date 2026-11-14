#include <fstream>
#include <iostream>
#include <omp.h>
#include <vector>
#include <windows.h>
#include <cstdlib> // Для rand()
#include <ctime>   // Для time()

using namespace std;

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");

    int numThreads;
    cout << "Введите количество потоков: ";
    cin >> numThreads;

    if (numThreads <= 0) {
        cout << "Ошибка: число потоков должно быть положительным!" << endl;
        return 1;
    }

    // АВТОСОЗДАНИЕ ФАЙЛА 
    string filename = "crd_big.txt";
    ifstream checkFile(filename);

    if (!checkFile.is_open()) {
        cout << "Файл " << filename << " не найден. Создаю новый файл на 100000 точек..." << endl;
        ofstream createFile(filename);

        srand(time(0)); // Инициализация генератора случайных чисел

        for (int i = 1; i <= 100000; i++) {
            // Генерируем случайные числа от -1000.0 до 1000.0
            // (rand() % 2000000) дает числа от 0 до 1999999
            // Делим на 1000.0 (получаем от 0.0 до 1999.999) и вычитаем 1000.0 (получаем от -1000.0 до 999.999)
            double x = (rand() % 2000000) / 1000.0 - 1000.0;
            double y = (rand() % 2000000) / 1000.0 - 1000.0;
            double z = (rand() % 2000000) / 1000.0 - 1000.0;

            createFile << x << " " << y << " " << z << "\n";
        }
        createFile.close();
        cout << "Файл успешно создан! Продолжаем...\n" << endl;
    }
    checkFile.close();

    // ЧТЕНИЕ ФАЙЛА 
    vector<double> Vx, Vy, Vz;
    double x, y, z;

    ifstream in(filename);
    while (in >> x >> y >> z) {
        Vx.push_back(x);
        Vy.push_back(y);
        Vz.push_back(z);
    }
    in.close();

    int N = Vx.size();
    if (N == 0) {
        cout << "Ошибка: Прочитано 0 точек." << endl;
        return 1;
    }

    cout << "Прочитано точек: " << N << endl << endl;

    //  ВЫЧИСЛЕНИЕ (Задача 6) 
    double global_sum = 0.0; // Глобальная сумма для всех координат
    omp_set_num_threads(numThreads);

    double start = omp_get_wtime();

#pragma omp parallel sections
    {
        // Секция 1: Сумма по X
#pragma omp section
        {
            for (int i = 0; i < N; i++) {
#pragma omp critical
                {
                    global_sum += Vx[i];
                }
            }
        }

        // Секция 2: Сумма по Y
#pragma omp section
        {
            for (int i = 0; i < N; i++) {
#pragma omp critical
                {
                    global_sum += Vy[i];
                }
            }
        }

        // Секция 3: Сумма по Z
#pragma omp section
        {
            for (int i = 0; i < N; i++) {
#pragma omp critical
                {
                    global_sum += Vz[i];
                }
            }
        }
    }

    double end = omp_get_wtime();

    // Вычисляем по формуле: (Σx + Σy + Σz) / 3N
    double result = global_sum / (3.0 * N);

    cout << "Задача 6" << endl;
    cout << "Результат по формуле: " << result << endl;
    cout << "Время выполнения: " << end - start << " сек" << endl;

    return 0;
}
