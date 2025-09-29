#include <iostream>
#include "permutation.h"
#include "stierlitz.h"
#include "sparse_matrix.h"
using namespace std;

int main() {
    setlocale(0, "");
    int option;

    cout << "1. Проверка permutation\n";
    cout << "2. Расшифровка stierlitz\n";
    cout << "3. Разряженная матрица sparse-matrix\n";
    cout << "0. Выход\n";

    while (true) {

        cout << "\nваш выбор: ";
        cin >> option;

        if (option == 1) {
            runPermutation();
        }
        else if (option == 2) {
            runStierlitz();
        }
        else if (option == 3) {
            runSparseMatrix();
        }
        else if (option == 0) {
            cout << "Выход из программы..." << endl;
            break;
        }
        else {
            cout << "Неверный выбор" << endl;
        }
    }
}
