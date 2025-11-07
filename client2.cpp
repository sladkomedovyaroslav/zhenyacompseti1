#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
using namespace std;

int main() {
    setlocale(LC_ALL, "rus");

    // Имя файла, через который клиент сообщает серверу о новом запросе
    const string reqFile = "C:/Users/s0177102/source/repos/file/queue.txt";

    // Клиент работает в бесконечном цикле — можно вводить студентов один за другим
    while (true) {
        string lastname;
        int exams[4];

        // Ввод фамилии студента
        cout << "\nВведите фамилию: ";
        cin >> lastname;

        // Ввод 4 оценок
        cout << "Введите 4 оценки: ";
        for (int i = 0; i < 4; ++i)
            cin >> exams[i];

        // Создание файла для конкретного студента
        string studentFile = "C:/Users/s0177102/source/repos/file/" + lastname + ".txt";

        // Записываем оценки в файл
        ofstream fout(studentFile);
        for (int i = 0; i < 4; ++i)
            fout << exams[i] << " ";
        fout.close();

        // Уведомляем сервер, записав фамилию в очередь
        ofstream foutReq(reqFile, ios::app);
        foutReq << lastname << endl;
        foutReq.close();

        cout << "[CLIENT] Отправлено на сервер..." << endl;

        // Определяем исходный размер файла (до ответа)
        streampos sizeBefore = 0;
        ifstream fcheck(studentFile, ios::ate);
        sizeBefore = fcheck.tellg();
        fcheck.close();

        // Ждём, пока файл не увеличится (сервер добавит ответ)
        bool updated = false;
        while (!updated) {
            Sleep(800); // Периодическая проверка

            // Проверяем размер файла снова
            ifstream fcheck2(studentFile, ios::ate);
            streampos sizeAfter = fcheck2.tellg();
            fcheck2.close();

            // Если размер стал больше — значит, сервер уже записал результат
            if (sizeAfter > sizeBefore) {
                ifstream fin(studentFile);
                string line;
                while (getline(fin, line))
                    if (line.find("РЕЗУЛЬТАТ") != string::npos)
                        cout << "[CLIENT] " << line << endl;
                fin.close();
                updated = true;
            }
        }
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

// Структура пакета данных для передачи на сервер
struct DataPacket {
    char name[32];
    int marks[4];
};

int main() {
    setlocale(LC_ALL, "rus");

    // Общий бинарный файл запросов
    const string queuePath = "C:/Users/s0177102/source/repos/file/requests_queue.bin";

    while (true) {
        string surname;
        DataPacket pkt = {}; // создаём пустую структуру

        // Ввод фамилии
        cout << "\nВведите фамилию студента: ";
        cin >> surname;

        // Копируем строку в char-массив (для бинарной записи)
        strncpy(pkt.name, surname.c_str(), sizeof(pkt.name) - 1);

        // Ввод 4 оценок
        cout << "Введите 4 оценки: ";
        for (int i = 0; i < 4; ++i)
            cin >> pkt.marks[i];

        // Записываем структуру в общий бинарный файл
        ofstream fout(queuePath, ios::binary | ios::app);
        fout.write(reinterpret_cast<char*>(&pkt), sizeof(DataPacket));
        fout.close();

        cout << "[CLIENT] Данные отправлены серверу. Ждём файл-ответ..." << endl;

        // Формируем имя выходного файла, который создаст сервер
        string outFile = "C:/Users/s0177102/source/repos/file/" + surname + "_out.bin";
        bool received = false;

        // Ожидаем, пока сервер создаст файл с ответом
        while (!received) {
            Sleep(1000);
            ifstream fin(outFile, ios::binary);
            if (fin.is_open()) {
                // Если файл появился — считываем ответ
                string response;
                getline(fin, response, '\0');
                cout << "[CLIENT] Ответ сервера: " << response << endl;
                fin.close();

                // Удаляем файл после чтения (чтобы не мешал следующему запросу)
                remove(outFile.c_str());
                received = true;
            }
        }
    }

    return 0;
}






__ 
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
