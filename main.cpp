#include "lab_11_2.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// === Допоміжні функції для демонстрації ===

void CreateTXT(const string& filename) {
    // !!! ВИПРАВЛЕНО: Використання cout замість cerr !!!
    ofstream fout(filename);
    if (!fout.is_open()) {
        cout << "Помилка створення/відкриття файлу " << filename << endl;
        return;
    }

    string line;
    cout << "Введіть рядки для файлу " << filename << " (введіть 'q' в окремому рядку для завершення):\n";

    // Введення рядків, поки користувач не введе 'q'
    while (getline(cin, line) && line != "q") {
        fout << line << endl;
    }
    fout.close();

    cin.clear();
}

void PrintTXT(const string& filename) {
    // !!! ВИПРАВЛЕНО: Використання cout замість cerr !!!
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Помилка відкриття файлу " << filename << " для читання." << endl;
        return;
    }

    string line;
    cout << "\n=== Вміст файлу " << filename << " ===\n";
    while (getline(fin, line)) {
        cout << line << endl;
    }
    cout << "================================\n";
    fin.close();
}

// ==========================================

int main() {
    string t1_filename = "t1_input.txt";
    string t2_filename = "t2_output.txt";

    // 1. Створення вхідного файлу t1
    cout << "Створення вхідного файлу t1_input.txt.\n";
    CreateTXT(t1_filename);

    // 2. Виведення вмісту t1
    PrintTXT(t1_filename);

    // 3. Виконання завдання: трансформація та запис у t2
    cout << "\nВиконую трансформацію файлу: t1 -> t2...\n";
    if (TransformTXT(t1_filename, t2_filename)) {
        cout << "Трансформація успішна. Результат записано у файл " << t2_filename << ".\n";

        // 4. Виведення вмісту t2
        PrintTXT(t2_filename);
    } else {
        // !!! ВИПРАВЛЕНО: Використання cout замість cerr !!!
        cout << "Трансформація завершилася з помилкою.\n";
    }

    return 0;
}