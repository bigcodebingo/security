#include <iostream>
#include "permutation.h"
#include "stierlitz.h"
using namespace std;

int main() {
    setlocale(0, "");
    int option;

    cout << "1. проверка permutation\n";
    cout << "2. расшифровка stierlitz\n";
    cout << "0. выход\n";

    while (true) {
        
        cout << "\nваш выбор: ";
        cin >> option;

        if (option == 1) {
            runPermutation();
        }
        else if (option == 2) {
            runStierlitz();
        }
        else if (option == 0) {
            cout << "выход из программы..." << endl;
            break;
        }
        else {
            cout << "неверный выбор" << endl;
        }
    }
}
