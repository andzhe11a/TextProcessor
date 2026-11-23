// Файл: LineNumberDisplayer.cpp
#include "LineNumberDisplayer.h"
#include "TextDocument.h"
#include <iostream>

// //
// // Имплементация на метода display за LineNumberDisplayer.
// //
void LineNumberDisplayer::display(const TextDocument& doc) const {
    // // Обхождаме документа ред по ред, от първия (индекс 0) до последния.
    for (size_t i = 0; i < doc.getLineCount(); ++i) {
        // // Отпечатваме номера на реда (тъй като i е 0-базирано, добавяме 1),
        // // последван от точка, интервал и самия текст на реда.
        std::cout << i + 1 << ". " << doc.getLineText(i) << std::endl;
    }
}