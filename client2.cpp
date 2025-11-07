#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
using namespace std;

int main() {
    setlocale(LC_ALL, "rus");

    const string reqFile = "C:/Users/s0177102/source/repos/file/REQUEST.txt";

    while (true) {
        string surname;
        int marks[4];

        cout << "\nВведите фамилию учащегося: ";
        cin >> surname;

        cout << "Введите оценки за 4 экзамена (через пробел): ";
        for (int i = 0; i < 4; i++)
            cin >> marks[i];

        // Формируем путь к файлу клиента
        string filePath = "C:/Users/s0177102/source/repos/file/" + surname + ".txt";

        // Записываем оценки в файл
        ofstream fout(filePath);
        for (int i = 0; i < 4; i++) fout << marks[i] << " ";
        fout.close();

        // Уведомляем сервер через REQUEST.txt
        ofstream foutReq(reqFile, ios::app);
        foutReq << surname << endl;
        foutReq.close();

        cout << "Запрос передан серверу. Подождите, идёт обработка..." << endl;

        bool gotAnswer = false;
        string line;
        while (!gotAnswer) {
            Sleep(500);
            ifstream fin(filePath);
            while (getline(fin, line)) {
                if (line.find("Результат") != string::npos) {
                    cout << line << endl;
                    gotAnswer = true;
                    break;
                }
            }
            fin.close();
        }

        cout << "---------------------------------------" << endl;
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

int main() {
    setlocale(LC_ALL, "rus");
    const string reqFile = "C:/Users/s0177102/source/repos/file/REQUEST.bin";

    while (true) {
        string name;
        Student s = {};

        cout << "Введите фамилию студента: ";
        cin >> name;
        strncpy(s.surname, name.c_str(), sizeof(s.surname) - 1);

        cout << "Введите 4 оценки через пробел: ";
        for (int i = 0; i < 4; i++)
            cin >> s.grades[i];

        // Записываем структуру в бинарный файл студента
        string studentFile = "C:/Users/s0177102/source/repos/file/" + name + ".bin";
        ofstream fout(studentFile, ios::binary | ios::trunc);
        fout.write(reinterpret_cast<char*>(&s), sizeof(Student));
        fout.close();

        // Добавляем фамилию в бинарный файл запросов
        ofstream foutReq(reqFile, ios::binary | ios::app);
        foutReq.write(s.surname, sizeof(s.surname));
        foutReq.close();

        cout << "Запрос отправлен серверу. Ожидание ответа..." << endl;

        // Ожидаем ответ
        bool answered = false;
        while (!answered) {
            Sleep(500);
            ifstream fin(studentFile, ios::binary);
            if (!fin.is_open()) continue;

            fin.seekg(0, ios::end);
            streampos size = fin.tellg();
            if (size > sizeof(Student)) {
                fin.seekg(sizeof(Student));
                string answer;
                getline(fin, answer, '\0');
                cout << answer << endl;
                answered = true;
            }
            fin.close();
        }

        cout << "----------------------------------\n";
    }

    return 0;
}
