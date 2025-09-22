#include <iostream>
#include <string>
#include <fstream>
using namespace std;

string stierlitz(string word) {
    bool changed = true;
    while (changed && word.size() > 1) {
        changed = false;
        for (int i = 0; i < word.size() - 1; i++) {
            if (word[i] == word[i + 1]) {
                word.erase(i, 2);
                changed = true;
                break;
            }
        }
    }
    return word;
}

void runStierlitz() {
    int choice;
    string word;

    cout << "1. ����\n2. �������\n";
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        ifstream file("C:/Users/soch1van/Desktop/security/stierlitz.txt");
        getline(file, word);
        file.close();
    }
    else {
        cout << "������� ����� ��� �����������: ";
        getline(cin, word);
    }

    string decoded = stierlitz(word);
    cout << "\n�������� �����: " << word
        << "\n��������������� �����: " << decoded << endl;
}
