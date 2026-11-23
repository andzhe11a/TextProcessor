#include "CenterDisplayer.h"
#include "TextDocument.h"
#include <iostream>
#include <string>

CenterDisplayer::CenterDisplayer(size_t centerWidth) : width(centerWidth) {}

void CenterDisplayer::display(const TextDocument& doc) const {
    // // Обхождаме документа ред по ред.
    for (size_t i = 0; i < doc.getLineCount(); ++i) {
        std::string line = doc.getLineText(i);

        // // Проверяваме специален случай: ако редът е по-дълъг или равен на зададената ширина,
        // // няма как да го центрираме смислено, затова просто го отпечатваме директно.
        if (line.length() >= this->width) {
            std::cout << line << std::endl;
            continue; // 'continue' прескача останалата част от кода и преминава към следващия ред.
        }

        // // Изчисляваме необходимия брой интервали отляво ('padding'), за да се получи центриране.
        // // Намираме общото празно пространство (ширина - дължина на текста) и го делим на 2.
        size_t padding = (this->width - line.length()) / 2;

        // // Отпечатваме изчисления брой празни интервали, за да "бутнем" текста надясно.
        for (size_t p = 0; p < padding; ++p) {
            std::cout << " ";
        }

        // // Накрая отпечатваме самия текст, последван от символ за нов ред.
        std::cout << line << std::endl;
    }
}
