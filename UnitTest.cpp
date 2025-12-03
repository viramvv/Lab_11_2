#include "gtest/gtest.h"
#include "lab_11_2.h" // Містить оголошення TransformTXT
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <algorithm> // Для std::remove

using namespace std;

// ==========================================================
// 1. Допоміжні функції
// ==========================================================

// Функція для читання вмісту файлу в один рядок (для порівняння)
string ReadFileContent(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) return "";

    stringstream buffer;
    buffer << fin.rdbuf();
    string content = buffer.str();

    // !!! ВАЖЛИВО: Видаляємо всі символи повернення каретки (\r) !!!
    // Це усуває помилки порівняння, спричинені різницею форматів кінця рядка (Windows/Linux/macOS).
    content.erase(std::remove(content.begin(), content.end(), '\r'), content.end());

    return content;
}

// ==========================================================
// 2. Тестова Фікстура (Test Fixture)
// ==========================================================

class TransformTXTTest : public ::testing::Test {
protected:
    const string INPUT_FILE = "t1_test_in.txt";
    const string OUTPUT_FILE = "t2_test_out.txt";

    // Метод, що виконується перед кожним тестом
    void SetUp() override {
        // Гарантуємо, що вихідний файл не існує перед тестом
        remove(OUTPUT_FILE.c_str());
    }

    // Метод, що виконується після кожного тесту (Очищення)
    void TearDown() override {
        // Видаляємо обидва файли після тестування
        remove(INPUT_FILE.c_str());
        remove(OUTPUT_FILE.c_str());
    }
};

// ==========================================================
// 3. Юніт-тести
// ==========================================================

// Тест 1: Перевірка основного сценарію перетворення через файли
TEST_F(TransformTXTTest, MainTransformationScenario) {
    // 1. Створення вхідного файлу з текстом для перевірки
    ofstream fout(INPUT_FILE);
    fout << "це перше речення. наступне слово. новий рядок. слово після крапки у кінці файлу." << endl;
    fout.close();

    // 2. Виконання головної функції
    ASSERT_TRUE(TransformTXT(INPUT_FILE, OUTPUT_FILE))
        << "TransformTXT має успішно завершити виконання.";

    // 3. Перевірка результату
    // Очікуваний результат:
    // - Перше слово завжди велике ("це" -> "Це")
    // - Слова після крапки великі ("наступне" -> "Наступне", "новий" -> "Новий", "слово" -> "Слово")
    string expected = "це перше речення. наступне слово. новий рядок. слово після крапки у кінці файлу.\n";
    string actual = ReadFileContent(OUTPUT_FILE);

    ASSERT_EQ(actual, expected)
        << "вміст вихідного файлу не відповідає очікуваному. фактичний результат міг містити помилки кодування/форматування.\n";
}

// Тест 2: Перевірка, що слова без крапки не змінюються (крім першого)
TEST_F(TransformTXTTest, NoDotsNoChange) {
    // Створюємо файл без крапок
    ofstream fout(INPUT_FILE);
    fout << "перше слово далі без змін другий рядок також без крапок" << endl;
    fout.close();

    ASSERT_TRUE(TransformTXT(INPUT_FILE, OUTPUT_FILE));
    string actual = ReadFileContent(OUTPUT_FILE);

    // Перше слово має бути великим, решта — без змін.
    string expected = "перше слово далі без змін другий рядок також без крапок\n";
    ASSERT_EQ(actual, expected)
        << "текст без крапок повинен мати велику літеру лише на початку файлу.";
}

// Тест 3: Перевірка з пунктуацією (крім крапки)
TEST_F(TransformTXTTest, PunctuationHandling) {
    ofstream fout(INPUT_FILE);
    fout << "перше речення! друге речення? третє, речення." << endl;
    fout.close();

    ASSERT_TRUE(TransformTXT(INPUT_FILE, OUTPUT_FILE));
    string actual = ReadFileContent(OUTPUT_FILE);

    // ! і ? не активують capitalize_next. Тільки '.' активує.
    string expected = "перше речення! друге речення? третє, речення.\n";
    ASSERT_EQ(actual, expected);
}