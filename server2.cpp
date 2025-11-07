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



В БИНАРНОМ ВИДЕ

#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <cstring>
using namespace std;

struct Student {
    char surname[32];
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

    const string reqFile = "C:/Users/s0177102/source/repos/file/REQUEST.bin";
    char lastNameProcessed[32] = "";

    while (true) {
        Sleep(500);

        ifstream fin(reqFile, ios::binary);
        if (!fin.is_open()) continue;

        char current[32] = "";
        char latest[32] = "";

        // Читаем последнюю фамилию из бинарного файла
        while (fin.read(current, sizeof(current))) {
            strncpy(latest, current, sizeof(latest));
        }
        fin.close();

        if (strlen(latest) == 0 || strcmp(latest, lastNameProcessed) == 0)
            continue;

        strcpy(lastNameProcessed, latest);

        // Читаем бинарный файл студента
        string studentFile = "C:/Users/s0177102/source/repos/file/" + string(latest) + ".bin";
        ifstream finStudent(studentFile, ios::binary);
        if (!finStudent.is_open()) continue;

        Student s;
        finStudent.read(reinterpret_cast<char*>(&s), sizeof(Student));
        finStudent.close();

        int result = getResult(s);

        cout << "Получены данные от " << s.surname << ". Результат вычислен: " << result << endl;

        string response = "\nРезультат проверки: ";
        switch (result) {
        case 0: response += "Студент отчислен"; break;
        case 1: response += "Стипендия не назначена"; break;
        case 2: response += "Назначена обычная стипендия"; break;
        case 3: response += "Назначена повышенная стипендия"; break;
        }

        // Записываем ответ в бинарный файл студента
        ofstream fout(studentFile, ios::binary | ios::app);
        fout.write(response.c_str(), response.size() + 1); // +1 для '\0'
        fout.close();
    }

    return 0;
}
