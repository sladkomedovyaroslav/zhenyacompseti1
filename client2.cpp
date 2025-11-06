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
