#include <fstream>
#include <iostream>
#include <omp.h>
#include <vector>
#include <windows.h>

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

    // АВТОСОЗДАНИЕ ФАЙЛА (чтобы решить проблему с путями) 
    string filename = "crd_big.txt";
    ifstream checkFile(filename);

    if (!checkFile.is_open()) {
        cout << "Файл " << filename << " не найден. Создаю новый файл на 100000 точек..." << endl;
        ofstream createFile(filename);
        for (int i = 1; i <= 100000; i++) {
            // Пишем уникальные целые числа: i, i+1, i+2
            createFile << i << " " << i + 1 << " " << i + 2 << "\n";
        }
        createFile.close();
        cout << "Файл успешно создан! Продолжаем...\n" << endl;
    }
    checkFile.close();
    // 

    // ЧТЕНИЕ ФАЙЛА 
    vector<double> Vx, Vy, Vz;
    double x, y, z;

    ifstream in(filename);
    if (!in.is_open()) {
        cout << "Критическая ошибка: Файл так и не открылся!" << endl;
        return 1;
    }

    while (in >> x >> y >> z) {
        Vx.push_back(x);
        Vy.push_back(y);
        Vz.push_back(z);
    }
    in.close();

    int N = Vx.size();

    // Защита от -nan(ind)
    if (N == 0) {
        cout << "Ошибка: Прочитано 0 точек. Проверьте файл!" << endl;
        return 1;
    }

    cout << "Прочитано точек: " << N << endl << endl;

    double sumi_x = 0, sumi_y = 0, sumi_z = 0;

    // ВЕРСИЯ 1: parallel for + reduction
    omp_set_num_threads(numThreads);

    double start1 = omp_get_wtime();

#pragma omp parallel for reduction(+:sumi_x, sumi_y, sumi_z)
    for (int i = 0; i < N; i++) {
        sumi_x += Vx[i];
        sumi_y += Vy[i];
        sumi_z += Vz[i];
    }

    double end1 = omp_get_wtime();

    cout << "=== Версия 1 (reduction) ===" << endl;
    cout << "Центр: " << sumi_x / N << " " << sumi_y / N << " " << sumi_z / N << endl;
    cout << "Время: " << end1 - start1 << " сек" << endl << endl;

    // ВЕРСИЯ 2: parallel sections

    sumi_x = 0; sumi_y = 0; sumi_z = 0;
    omp_set_num_threads(numThreads);

    double start2 = omp_get_wtime();

#pragma omp parallel sections
    {
#pragma omp section
        {
            for (int i = 0; i < N; i++) sumi_x += Vx[i];
        }

#pragma omp section
        {
            for (int i = 0; i < N; i++) sumi_y += Vy[i];
        }

#pragma omp section
        {
            for (int i = 0; i < N; i++) sumi_z += Vz[i];
        }
    }

    double end2 = omp_get_wtime();

    cout << "=== Версия 2 (sections) ===" << endl;
    cout << "Центр: " << sumi_x / N << " " << sumi_y / N << " " << sumi_z / N << endl;
    cout << "Время: " << end2 - start2 << " сек" << endl;

    return 0;
}
