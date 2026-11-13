#include <iostream>
#include <omp.h>

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    omp_set_num_threads(4);
    int all = omp_get_max_threads();

    ////////// 1 способ

    cout << "\n1 способ:\n";

#pragma omp parallel for ordered
    for (int i = 0; i < all; i++) {
#pragma omp ordered
        {
            int id = all - 1 - i;
            cout << "Поток " << id << " из " << all << endl;
        }
    }

    ////////// 2 способ

    cout << "\n2 способ:\n";

    int ids[100];  ///массив, в котором хранятся номера потоков.

#pragma omp parallel
    {
        int id = omp_get_thread_num();  ///каждый поток узнаёт свой номер.
        ids[id] = id; /// поток записывает номер в соответствующую ячейку массива.
    }

    for (int i = all - 1; i >= 0; i--) {
        cout << "Поток " << ids[i] << " из " << all << endl;
    }

    ////////// 3 способ

    cout << "\n3 способ:\n";

#pragma omp parallel
    {
        int id = omp_get_thread_num();

#pragma omp barrier

        for (int memb = all - 1; memb >= 0; memb--) {
            if (id == memb) {
#pragma omp critical
                {
                    cout << "Поток " << id << " из " << all << endl;
                }
            }

#pragma omp barrier
        }
    }

    ////////// 4 способ

    cout << "\n4 способ:\n";

    int maxid = all - 1;

#pragma omp parallel shared(maxid)
    {
        int id = omp_get_thread_num();

        for (int memb = all - 1; memb >= 0; memb--) {
#pragma omp barrier

            if (id == maxid) {
#pragma omp critical
                {
                    cout << "Поток " << id << " из " << all << endl;
                    maxid--;
                }
            }

#pragma omp barrier
        }
    }

    ////////// 5 способ

    cout << "\n5 способ:\n";

#pragma omp parallel
    {
        int id = omp_get_thread_num();

        for (int memb = all - 1; memb >= 0; memb--) {
            if (id == memb) {
                cout << "Поток " << id << " из " << all << endl;
            }

#pragma omp barrier
        }
    }

    return 0;
}
