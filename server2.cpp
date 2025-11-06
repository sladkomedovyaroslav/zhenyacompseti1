#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
using namespace std;

struct Student {
    string surname;
    int grades[4];
};

// Функция определения результата
int getResult(const Student& s) {
    bool has2 = false, has3 = false;
    int sum = 0;

    for (int i = 0; i < 4; i++) {
        if (s.grades[i] == 2) has2 = true;
        if (s.grades[i] == 3) has3 = true;
        sum += s.grades[i];
    }

    if (has2) return 0;       // отчислен
    if (has3) return 1;       // без стипендии
    if (sum == 20) return 3;  // повышенная
    return 2;                 // обычная
}

int main() {
    setlocale(LC_ALL, "rus");
    cout << "Сервер запущен и ожидает запросы от клиентов..." << endl;

    const string reqFile = "C:/Users/s0177102/source/repos/file/REQUEST.txt";
    string lastNameProcessed = "";

    while (true) {
        Sleep(500);
        ifstream fin(reqFile);
        if (!fin.is_open()) continue;

        string temp, latest;
        while (fin >> temp) latest = temp;
        fin.close();

        if (latest.empty() || latest == lastNameProcessed)
            continue;

        lastNameProcessed = latest;

        string studentFile = "C:/Users/s0177102/source/repos/file/" + latest + ".txt";
        ifstream finStudent(studentFile);
        if (!finStudent.is_open()) continue;

        Student s;
        s.surname = latest;
        for (int i = 0; i < 4; i++) finStudent >> s.grades[i];
        finStudent.close();

        int result = getResult(s);

        cout << "Данные получены от " << s.surname << ". Результат вычислен: " << result << endl;

        ofstream fout(studentFile, ios::app);
        fout << "\nРезультат проверки: ";
        switch (result) {
        case 0: fout << "Студент отчислен"; break;
        case 1: fout << "Стипендия не назначена"; break;
        case 2: fout << "Назначена обычная стипендия"; break;
        case 3: fout << "Назначена повышенная стипендия"; break;
        }
        fout.close();
    }

    return 0;
}
