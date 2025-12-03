#include "lab_11_2.h"
#include <fstream>
#include <iostream>
#include <cctype>
#include <string>
#include <algorithm> // Для std::transform (хоча ми його не використовуємо, залишаємо для загальної практики)

using namespace std;

// Допоміжна функція для безпечного перетворення першої літери слова на велику
// Вона працює з char, але застосовує toupper лише до першої літери, якщо це англійська літера
// або до першого байту UTF-8 символу (який може бути частиною літери).
char SafeToUpper(char c) {
    // toupper працює коректно лише для ASCII літер.
    // Для кирилиці (UTF-8) просто використовуємо toupper, сподіваючись, що він спрацює для першого байту
    // або залишаємо як є, якщо це не латинська літера.
    return (char)std::toupper((unsigned char)c);
}


/**
 * @brief Обробляє потік символів, змінюючи першу літеру слова на велику після крапки.
 */
bool ProcessFileStream(ifstream& t1_stream, ofstream& t2_stream) {
    // Початкове значення true, щоб перше слово у файлі також було з великої літери (типова вимога)
    bool capitalize_next = true;

    char ch;

    // Посимвольне читання файлу
    while (t1_stream.get(ch)) {

        // 1. Обробка пробілів
        if (isspace(ch)) {
            t2_stream.put(ch);
            continue; // Не скидаємо capitalize_next, якщо це пробіл
        }

        // 2. Обробка пунктуації
        if (ispunct(ch) && ch != '.') {
            // Це кома, дужка, тощо — просто записуємо
            t2_stream.put(ch);
            continue; // Не скидаємо capitalize_next
        }

        // 3. Обробка крапки
        if (ch == '.') {
            t2_stream.put(ch);
            capitalize_next = true; // Після крапки наступне слово має бути великим
            continue;
        }

        // 4. Обробка літер та інших символів (включаючи кирилицю)

        // Якщо це не пробіл і не пунктуація, і ми очікуємо велику літеру
        if (capitalize_next) {
            // Застосовуємо toupper до символу
            t2_stream.put(SafeToUpper(ch));
            capitalize_next = false; // Більше не очікуємо велику літеру до наступної крапки
        } else {
            // Просто записуємо символ як є
            t2_stream.put(ch);
        }
    }

    return true;
}


bool TransformTXT(const string& t1_filename, const string& t2_filename) {
    // Відкриття файлів
    ifstream t1_file(t1_filename);
    ofstream t2_file(t2_filename, ios::trunc);

    if (!t1_file.is_open()) {
        cout << "Помилка: Не вдалося відкрити вхідний файл " << t1_filename << endl;
        return false;
    }

    if (!t2_file.is_open()) {
        cout << "Помилка: Не вдалося відкрити вихідний файл " << t2_filename << endl;
        t1_file.close();
        return false;
    }

    // Обробка файлу
    bool result = ProcessFileStream(t1_file, t2_file);

    t1_file.close();
    t2_file.close();

    return result;
}