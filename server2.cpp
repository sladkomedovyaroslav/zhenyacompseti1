 
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <windows.h>
using namespace std;

// Структура для хранения данных о студенте
struct StudentInfo {
    string name;
    int exams[4];
};

// Функция расчёта типа стипендии или отчисления
int calcScholarship(const StudentInfo& st) {
    bool has2 = false, has3 = false;
    int total = 0;

    // Перебираем оценки и подсчитываем сумму
    for (int i = 0; i < 4; ++i) {
        if (st.exams[i] == 2) has2 = true;
        if (st.exams[i] == 3) has3 = true;
        total += st.exams[i];
    }

    // Возвращаем код результата
    if (has2) return 0;   // Отчислен
    if (has3) return 1;   // Без стипендии
    if (total == 20) return 3; // Повышенная стипендия
    return 2;             // Обычная стипендия
}

int main() {
    setlocale(LC_ALL, "rus");
    cout << "[SERVER] Стартует текстовый сервер..." << endl;

    // Файл, куда клиенты записывают фамилии студентов
    const string reqFile = "C:/Users/s0177102/source/repos/file/queue.txt";

    // Множество обработанных фамилий (чтобы не повторять обработку)
    set<string> processed;

    // Бесконечный цикл — сервер постоянно проверяет наличие новых записей
    while (true) {
        Sleep(1000); // Пауза между проверками (1 секунда)

        ifstream fin(reqFile);
        if (!fin.is_open()) continue; // Если файл не найден — продолжаем ждать

        string name;
        while (fin >> name) { // Считываем фамилии из файла по очереди
            if (processed.count(name)) continue; // Пропускаем, если уже обрабатывали
            processed.insert(name); // Добавляем фамилию в список обработанных

            // Формируем путь к файлу с оценками студента
            string filePath = "C:/Users/s0177102/source/repos/file/" + name + ".txt";
            ifstream dataIn(filePath);
            if (!dataIn.is_open()) continue; // Если файла нет — пропускаем

            // Считываем оценки студента
            StudentInfo st;
            st.name = name;
            for (int i = 0; i < 4; ++i) dataIn >> st.exams[i];
            dataIn.close();

            // Вычисляем результат
            int code = calcScholarship(st);

            // Формируем текстовое сообщение
            string result;
            if (code == 0) result = "Отчислен";
            else if (code == 1) result = "Без стипендии";
            else if (code == 2) result = "Обычная стипендия";
            else result = "Повышенная стипендия";

            // Добавляем результат в файл студента
            ofstream dataOut(filePath, ios::app);
            dataOut << "\nРЕЗУЛЬТАТ: " << result;
            dataOut.close();

            cout << "[SERVER] Обработан " << name << " → " << result << endl;
        }
        fin.close();
    }

    return 0;
}

В БИНАРНОМ ВИДЕ

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <cstring>
using namespace std;

// Структура для передачи данных студента
struct DataPacket {
    char name[32];
    int marks[4];
};

// Функция оценки результатов
int evaluate(const DataPacket& d) {
    bool fail = false, weak = false;
    int total = 0;

    // Анализ оценок
    for (int i = 0; i < 4; ++i) {
        total += d.marks[i];
        if (d.marks[i] == 2) fail = true;
        if (d.marks[i] == 3) weak = true;
    }

    // Код результата
    if (fail) return 0;
    if (weak) return 1;
    if (total == 20) return 3;
    return 2;
}

int main() {
    setlocale(LC_ALL, "rus");
    cout << "[SERVER] Бинарный сервер работает..." << endl;

    // Общий бинарный файл, куда клиенты записывают структуры запросов
    const string reqPath = "C:/Users/s0177102/source/repos/file/requests_queue.bin";

    // Размер файла при последней проверке
    streampos lastSize = 0;

    // Основной цикл обработки
    while (true) {
        Sleep(1000); // Проверка раз в секунду

        // Проверяем текущий размер файла
        ifstream fin(reqPath, ios::binary | ios::ate);
        if (!fin.is_open()) continue;

        streampos currentSize = fin.tellg();
        if (currentSize == lastSize) { // ничего не изменилось
            fin.close();
            continue;
        }

        // Если появились новые данные — читаем их
        fin.seekg(lastSize); // переходим к новому месту
        DataPacket pkt;

        // Считываем все новые структуры
        while (fin.read(reinterpret_cast<char*>(&pkt), sizeof(DataPacket))) {
            // Формируем имя файла для клиента
            string clientFile = "C:/Users/s0177102/source/repos/file/" + string(pkt.name) + ".bin";

            // Вычисляем результат
            int code = evaluate(pkt);

            // Готовим текст ответа
            string answer;
            switch (code) {
            case 0: answer = "Отчислен"; break;
            case 1: answer = "Без стипендии"; break;
            case 2: answer = "Обычная стипендия"; break;
            case 3: answer = "Повышенная стипендия"; break;
            }

            // Записываем ответ в отдельный бинарный файл
            string outFile = "C:/Users/s0177102/source/repos/file/" + string(pkt.name) + "_out.bin";
            ofstream fout(outFile, ios::binary | ios::trunc);
            fout.write(answer.c_str(), answer.size() + 1);
            fout.close();

            cout << "[SERVER] Ответ сформирован для " << pkt.name << ": " << answer << endl;
        }

        // Обновляем контроль размера
        lastSize = currentSize;
        fin.close();
    }

    return 0;
}
