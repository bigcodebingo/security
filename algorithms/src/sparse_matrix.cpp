#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

map<pair<int, int>, string> access_matrix;
map<int, string> users;

int next_id = 1;
int N = 0, M = 0;

void add_user(const string& name) {
    if ((int)users.size() >= N) {
        cout << "Нельзя добавить: число субъектов уже достигло " << N << endl << endl;
        return;
    }
    int id = next_id++;
    users[id] = name;
    cout << name << " зарегестрирован как " << id << endl << endl;
}

void delete_user(int id) {
    if (users.count(id)) {
        users.erase(id);
        vector<pair<int, int>> to_delete;
        for (auto& cell : access_matrix) {
            if (cell.first.first == id) to_delete.push_back(cell.first);
        }
        for (auto& key : to_delete) access_matrix.erase(key);
        cout << "Пользователь " << id << " удален" << endl << endl;
    }
    else {
        cout << "Пользователь не найден " << id << endl << endl;
    }
}

void edit_cell(int user_id, int obj_id, const string& rights) {
    string current = access_matrix[{user_id, obj_id}];
    if (rights == "r+") {
        if (current.find('r') == string::npos) current += "r";
    }
    else if (rights == "w+") {
        if (current.find('w') == string::npos) current += "w";
    }
    else if (rights == "r-") {
        current.erase(remove(current.begin(), current.end(), 'r'), current.end());
    }
    else if (rights == "w-") {
        current.erase(remove(current.begin(), current.end(), 'w'), current.end());
    }
    if (current.empty()) access_matrix.erase({ user_id,obj_id });
    else {
        string ordered;
        if (current.find('r') != string::npos) ordered += 'r';
        if (current.find('w') != string::npos) ordered += 'w';
        access_matrix[{user_id, obj_id}] = ordered;
        current = ordered;
    }
    cout << "Права пользователя " << user_id << " на объект " << obj_id << ": " << current << endl << endl;
}

void percent_check() {
    int total = N * M;
    int filled = access_matrix.size();
    double percent = (double)filled / total * 100.0;
    cout << "Процент заполненности матрицы: " << percent << "%\n" << endl;
}

void list_access(int obj) {
    if (obj < 1 || obj > M) {
        cout << "Файл с номером " << obj << " не существует" << endl << endl;
        return;
    }

    cout << "Пользователи с доступом к файлу " << obj << ": ";
    bool found = false;
    for (auto& cell : access_matrix) {
        if (cell.first.second == obj) {
            int subj = cell.first.first;
            cout << users[subj] << "(user_id:" << subj << ") ";
            found = true;
        }
    }
    if (!found) {
        cout << "нет пользователей с доступом";
    }
    cout << endl << endl;
}

void load_from_file(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cerr << "Ошибка при открытии файла " << filename << endl << endl;
        return;
    }

    access_matrix.clear();
    users.clear();

    fin >> N >> M;
    fin.ignore();

    string line;
    bool in_users = false, in_rights = false;

    while (getline(fin, line)) {
        if (line.empty()) continue;

        if (line == "users") { in_users = true; in_rights = false; continue; }
        if (line == "rights") { in_users = false; in_rights = true; continue; }

        stringstream ss(line);

        if (in_users) {
            int user_id; string username;
            ss >> user_id >> username;
            users[user_id] = username;
            next_id = max(next_id, user_id + 1);
        }
        else if (in_rights) {
            int user_id, object_id; string user_rights;
            ss >> user_id >> object_id >> user_rights;
            access_matrix[{user_id, object_id}] = user_rights;
        }
    }
    cout << "Данные загружены из " << filename << endl << endl;
    fin.close();
}

void save_to_file(const string& filename) {
    ofstream fout(filename);
    if (!fout) {
        cerr << "Ошибка при сохранении файла " << filename << endl << endl;
        return;
    }

    fout << N << " " << M << "\n";

    fout << "users\n";
    for (auto& user : users) { fout << user.first << " " << user.second << "\n"; }

    fout << "rights\n";
    for (auto& cell : access_matrix) {
        fout << cell.first.first << " " << cell.first.second << " " << cell.second << "\n";
    }
    fout.close();
    cout << "Данные сохранены в " << filename << endl << endl;
}

void runSparseMatrix() {
    string cmd;
    cout << "Используйте команды: (add/delete/edit/list/percent/load/save/exit):\n" << endl;

    while (cin >> cmd) {
        if (cmd == "add") {
            string name;
            cin >> name;
            add_user(name);
        }
        else if (cmd == "delete") {
            int id;
            cin >> id;
            delete_user(id);
        }
        else if (cmd == "edit") {
            int user_id, obj_id;
            string rights;
            cin >> user_id >> obj_id >> rights;
            edit_cell(user_id, obj_id, rights);
        }
        else if (cmd == "percent") {
            percent_check();
        }
        else if (cmd == "list") {
            int obj;
            cin >> obj;
            list_access(obj);
        }
        else if (cmd == "load") {
            load_from_file("C:/Users/soch1van/Desktop/security/matrix.txt");
        }
        else if (cmd == "save") {
            save_to_file("C:/Users/soch1van/Desktop/security/matrix.txt");
        }
        else if (cmd == "exit") {
            cout << "Выход из режима разряженной матрицы.\n\n";
            break;
        }
        else {
            cout << "Неверная команда!\n" << endl;
        }
    }
}