#include "chinese_wall.h"

map<int, string> subjects;                
map<int, string> objects;

map<int, string> object_firm;            
map<string, set<int>> firm_objects;     

map<string, set<string>> conflict_classes; 
map<int, vector<Operation>> subj_operation_history;
map<int, set<string>> subj_firms_history;
map<int, set<string>> subj_classes_history;

void add_subject(int id, const string& name) { subjects[id] = name; }

void add_object(int id, const string& name, const string& firm) {
    objects[id] = name;
    object_firm[id] = firm;
    firm_objects[firm].insert(id);
}
void add_conflict_class(const string& class_name, const vector<string>& firms) {
    conflict_classes[class_name] = set<string>(firms.begin(), firms.end());
}

void start() {
    subj_operation_history.clear();
    subj_firms_history.clear();
    subj_classes_history.clear();
    cout << "История доступа очищена.\n\n";
}

bool read_object(int subj_id, int obj_id) {
    if (!subjects.count(subj_id) || !objects.count(obj_id)) return false;

    string firm = object_firm[obj_id];

    for (auto& [class_name, firms] : conflict_classes) {
        if (firms.count(firm) && subj_firms_history[subj_id].size()) {
            for (const string& f : subj_firms_history[subj_id]) {
                if (f != firm && firms.count(f)) {
                    cout << "Отказ в чтении.\n" << endl;
                    return false;
                }
            }
        }
    }

    subj_operation_history[subj_id].push_back({"read", obj_id, firm});
    subj_firms_history[subj_id].insert(firm);

    for (auto& [class_name, firms] : conflict_classes) {
        if (firms.count(firm)) subj_classes_history[subj_id].insert(class_name);
    }

    cout << "Файл успешно прочитан!\n" << endl;
    return true;
}

bool write_object(int subj_id, int obj_id) {
    if (!read_object(subj_id, obj_id)) {return false;}

    string firm = object_firm[obj_id];

    for (const auto& [class_name, firms] : conflict_classes) {
        if (firms.count(firm)) { 
            for (const auto& f : firms) {
                if (f != firm && subj_firms_history[subj_id].count(f)) {
                    cout << "Отказано в записи.\n" << endl;
                    return false;
                }
            }
        }
    }

    subj_operation_history[subj_id].push_back({ "write", obj_id, firm });
    cout << "Успешно записано в файл!\n" << endl;
    return true;
}

void report_subject(int subj_id) {
    if (!subjects.count(subj_id)) {
        cout << "Субъект не найден.\n";
        return;
    }
    cout << "Отчёт по субъекту " << subjects[subj_id] << ":\n";
    for (auto& op : subj_operation_history[subj_id]) {
        cout << op.type << " " << objects[op.object_id] << " (" << op.firm << ")\n";
    }
    cout << endl;
}

void report_object(int obj_id) {
    if (!objects.count(obj_id)) {
        cout << "Объект не найден.\n";
        return;
    }
    cout << "Отчёт по объекту " << objects[obj_id] << ":\n";
    for (auto& [subj_id, ops] : subj_operation_history) {
        for (auto& op : ops) {
            if (op.object_id == obj_id) {
                cout << subjects[subj_id] << " " << op.type << "\n";
            }
        }
    }
    cout << endl;
}

void brief_case(const string& firm_name) {
    if (!firm_objects.count(firm_name)) {
        cout << "Фирма не найдена.\n";
        return;
    }
    cout << "Объекты фирмы " << firm_name << ":\n";
    for (int obj_id : firm_objects[firm_name]) {
        cout << objects[obj_id] << " ";
    }
    cout << endl << endl;
}

void load_info(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cerr << "Ошибка при открытии файла " << filename << endl << endl;
        return;
    }

    subjects.clear();
    objects.clear();
    object_firm.clear();
    firm_objects.clear();
    conflict_classes.clear();
    subj_operation_history.clear();
    subj_firms_history.clear();
    subj_classes_history.clear();

    string line;
    string section;

    while (getline(fin, line)) {
        if (line.empty()) continue;
        if (line == "subjects") { section = "subjects"; continue; }
        if (line == "firms") { section = "firms"; continue; }
        if (line == "objects") { section = "objects"; continue; }
        if (line == "conflict_classes") { section = "conflict_classes"; continue; }

        stringstream ss(line);

        if (section == "subjects") {
            int id; string name;
            ss >> id >> name;
            subjects[id] = name;
        }
        else if (section == "firms") {
            string firm;
            ss >> firm;
            firm_objects[firm]; 
        }
        else if (section == "objects") {
            int id; string name, firm;
            ss >> id >> name >> firm;
            objects[id] = name;
            object_firm[id] = firm;
            firm_objects[firm].insert(id);
        }
        else if (section == "conflict_classes") {
            string class_name, firm;
            ss >> class_name;
            while (ss >> firm) {
                conflict_classes[class_name].insert(firm);
            }
        }
    }

    fin.close();
    cout << "Данные загружены из " << filename << endl << endl;
}

void runChineseWall() {
    cout << "\nИспользуйте команды:\n"
        "load / start / read <subject_id> <object_id> / write <subject_id> <object_id> /\n"
        "reports <subject_id> / report_o <object_id> / brief_case <firm> / exit\n" << endl;

    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string cmd;
        ss >> cmd;

        if (cmd == "load") {
            load_info("C:/Users/soch1van/Desktop/security/test_wall.txt");
        }
        else if (cmd == "start") {
            subj_operation_history.clear();
            subj_firms_history.clear();
            subj_classes_history.clear();
            cout << "История доступов очищена.\n\n";
        }
        else if (cmd == "read") {
            int s, o;
            if (ss >> s >> o) read_object(s, o);
            else cout << "Неверная команда!\n\n";
        }
        else if (cmd == "write") {
            int s, o;
            if (ss >> s >> o) write_object(s, o);
            else cout << "Неверная команда!\n\n";
        }
        else if (cmd == "report_s") {
            string param;
            if (ss >> param) {
                try {
                    int id = stoi(param);
                    report_subject(id);
                }
                catch (const invalid_argument& e) {
                    cout << "Ошибка: параметр не является числом.\n\n";
                }
                catch (const out_of_range& e) {
                    cout << "Ошибка: число слишком большое.\n\n";
                }
            }
            else {
                cout << "Неверная команда! Используйте: report_s <id>\n\n";
            }
        }
        else if (cmd == "report_o") {
            string param;
            if (ss >> param) {
                try {
                    int id = stoi(param);
                    report_object(id);
                }
                catch (const invalid_argument& e) {
                    cout << "Ошибка: параметр не является числом\n\n";
                }
                catch (const out_of_range& e) {
                    cout << "Ошибка: число слишком большое\n\n";
                }
            }
            else {
                cout << "Неверная команда! Используйте: report_o <id>\n\n";
            }
        }
        else if (cmd == "brief_case") {
            string firm;
            if (ss >> firm) brief_case(firm);
            else cout << "Неверная команда!\n\n";
        }
        else if (cmd == "exit") {
            cout << "Выход из режима Chinese Wall.\n\n";
            break;
        }
        else {
            cout << "Введите команду из списка!\n\n";
        }
    }
}
