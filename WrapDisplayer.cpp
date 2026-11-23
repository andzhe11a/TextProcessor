#include "WrapDisplayer.h"
#include "TextDocument.h"
#include <iostream>
#include <string>

WrapDisplayer::WrapDisplayer(size_t wrapWidth) : width(wrapWidth) {}

void WrapDisplayer::display(const TextDocument& doc) const {
    // // Обхождаме документа ред по ред.
    for (size_t i = 0; i < doc.getLineCount(); ++i) {
        std::string line = doc.getLineText(i);

        // // Специален случай за празни редове - просто отпечатваме нов ред,
        // // за да запазим празното пространство във визуализацията.
        if (line.empty()) {
            std::cout << std::endl;
            continue;
        }

        // // 'current_pos' следи откъде да започнем да режем следващото парче от реда.
        size_t current_pos = 0;
        // // Вътрешен цикъл, който се изпълнява, докато не сме отпечатали целия ред.
        while (current_pos < line.length()) {
            // // Използваме 'substr', за да изрежем парче от реда.
            // // Парчето започва от 'current_pos' и е с дължина 'this->width'.
            // // 'substr' автоматично се справя, ако оставащият текст е по-къс от 'width'.
            std::cout << line.substr(current_pos, this->width) << std::endl;
            // // Преместваме позицията напред за следващото "парче".
            current_pos += this->width;
        }
    }
}