#include <iostream>
#include <fstream>
#include <string>
using namespace std;

bool isPermutation(const string& s, int n) {
    if (s.size() != n) return false;
    bool used[26] = { false };
    for (char c : s) {
        int idx = c - 'A';
        if (idx < 0 || idx >= n || used[idx]) return false;
        used[idx] = true;
    }
    return true;
}

bool isAlmostPermutation(const string& s, int n) {
    if (s.size() != n + 1) return false;
    for (int i = 0; i < s.size(); i++) {
        string t = s.substr(0, i) + s.substr(i + 1);
        if (isPermutation(t, n)) return true;
    }
    return false;
}

bool isHyperAlmostPermutation(const string& s, int n) {
    if (s.size() < n + 1) return false;
    for (int i = 0; i + n < s.size(); i++) {
        if (!isAlmostPermutation(s.substr(i, n + 1), n))
            return false;
    }
    return true;
}

string mergeStrings(const string& a, const string& b) {
    int maxOverlap = 0;
    for (int len = min(a.size(), b.size()); len > 0; len--) {
        if (a.substr(a.size() - len) == b.substr(0, len)) {
            maxOverlap = len;
            break;
        }
    }
    return a + b.substr(maxOverlap);
}

int main() {
    setlocale(0, "");
    ifstream fin("C:/Users/soch1van/Desktop/security/input.txt");
    ofstream fout("C:/Users/soch1van/Desktop/security/output.txt");

    int n;
    string s1, s2;
    fin >> n >> s1 >> s2;
    fin.close();

    string candidate = mergeStrings(s1, s2);
    if (!isHyperAlmostPermutation(candidate, n)) {
        candidate = mergeStrings(s2, s1);
        if (!isHyperAlmostPermutation(candidate, n)) {
            cout << "строки некорректны";
            return 0;   
        }
    }
    fout << candidate;
    fout.close();
}