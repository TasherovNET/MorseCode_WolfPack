#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <chrono>
#include <windows.h> // Для SendInput
#include <locale>

// Функция для перевода текста в азбуку Морзе
std::string textToMorse(const std::string& text) {
    std::map<char, std::string> morseCode = {
        {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."},
        {'E', "."}, {'F', "..-."}, {'G', "--."}, {'H', "...."},
        {'I', ".."}, {'J', ".---"}, {'K', "-.-"}, {'L', ".-.."},
        {'M', "--"}, {'N', "-."}, {'O', "---"}, {'P', ".--."},
        {'Q', "--.-"}, {'R', ".-."}, {'S', "..."}, {'T', "-"},
        {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"},
        {'Y', "-.--"}, {'Z', "--.."},
        {'1', ".----"}, {'2', "..---"}, {'3', "...--"}, {'4', "....-"},
        {'5', "....."}, {'6', "-...."}, {'7', "--..."}, {'8', "---.."},
        {'9', "----."}, {'0', "-----"},
        {' ', "/"} // Пробел обозначаем как "/"
    };

    std::string morse;
    for (char ch : text) {
        ch = toupper(ch);
        if (morseCode.find(ch) != morseCode.end()) {
            morse += morseCode[ch] + " "; // Добавляем пробел между буквами
        }
    }
    return morse;
}

// Функция для имитации нажатия клавиш
void simulateKeyPress(char key) {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;

    // Установка кода нажатия клавиши
    if (key == '.') {
        input.ki.wVk = VK_DECIMAL; // Код клавиши '.'
    }
    else if (key == '-') {
        input.ki.wVk = VK_SUBTRACT; // Код клавиши '-'
    }

    // Нажатие клавиши
    SendInput(1, &input, sizeof(INPUT));
    // Отпускание клавиши
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void sendMessage(const std::string& morseMessage) {
    std::string currentLetter;

    for (char ch : morseMessage) {
        if (ch == '.' || ch == '-') {
            currentLetter += ch; // Собираем текущую букву
        }
        else if (ch == ' ') {
            // Если встретили пробел, значит буква закончилась
            if (!currentLetter.empty()) {
                // Имитация нажатия для текущей буквы
                for (char morseChar : currentLetter) {
                    simulateKeyPress(morseChar);
                }
                std::cout << "Отправлена буква в Морзе: " << currentLetter << std::endl;
                currentLetter.clear(); // Очищаем текущую букву
                std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Пауза
            }
        }
    }

    // Если осталась последняя буква без пробела в конце
    if (!currentLetter.empty()) {
        for (char morseChar : currentLetter) {
            simulateKeyPress(morseChar);
        }
        std::cout << "Отправлена буква в Морзе: " << currentLetter << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Пауза 
    }
}

int main() {
    std::setlocale(LC_ALL, "Russian");
    std::cout << "Введите сообщение для отправки: ";
    std::string message;
    std::getline(std::cin, message);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    std::string morseMessage = textToMorse(message);
    std::cout << "Сообщение в Морзе: " << morseMessage << std::endl;
    sendMessage(morseMessage); // Отправка сообщения
    return main();
}