#include "SortCommand.h"
#include "TextDocument.h"

// // Помощна структура, която използваме, за да улесним сортирането.
// // Тя "опакова" текста на един ред заедно с информация дали той е числов.
// // Така избягваме да викаме isNumeric() многократно вътре в цикъла за сортиране.
struct SortableLine {
    std::string text;
    bool isNumeric;
};

SortCommand::SortCommand(TextDocument* doc) : doc(doc) {}

void SortCommand::execute() {
    // // Етап 1: Правим пълно резервно копие на документа в 'originalLines'.
    originalLines.clear();
    size_t lineCount = doc->getLineCount();
    for (size_t i = 0; i < lineCount; ++i) {
        originalLines.push_back(doc->getLineText(i));
    }

    // // Етап 2: Извличаме само подвижните редове в специален списък за сортиране.
    std::vector<SortableLine> movableLines;
    for (size_t i = 0; i < lineCount; ++i) {
        BaseLine* line = doc->getLineObject(i);
        // // Използваме полиморфните методи isMovable() и isNumeric(), за да съберем нужната информация.
        if (line != nullptr && line->isMovable()) {
            movableLines.push_back({line->getText(), line->isNumeric()});
        }
    }

    // // Етап 3: Сортираме списъка с подвижните редове.
    // // Използваният алгоритъм тук е "Bubble Sort" (метод на мехурчето).
    for (size_t i = 0; i < movableLines.size(); ++i) {
        for (size_t j = 0; j < movableLines.size() - 1 - i; ++j) {
            SortableLine& a = movableLines[j];
            SortableLine& b = movableLines[j + 1];
            bool shouldSwap = false;

            // // Тук е специалната логика за сравнение, според изискванията на проекта.
            // // Ако и двата реда са числа...
            if (a.isNumeric && b.isNumeric) {
                // // ...сравняваме ги по тяхната числена стойност.
                if (std::stoll(a.text) > std::stoll(b.text)) {
                    shouldSwap = true;
                }
            } else { // // Ако поне единият не е число...
                // // ...сравняваме ги стандартно, като текст (лексикографски).
                if (a.text > b.text) {
                    shouldSwap = true;
                }
            }
            // // Ако трябва, разменяме местата на двата елемента.
            if (shouldSwap) {
                SortableLine temp = a;
                a = b;
                b = temp;
            }
        }
    }

    // // Етап 4: Връщаме сортираните редове обратно в документа.
    int movable_idx = 0;
    for (size_t i = 0; i < lineCount; ++i) {
        BaseLine* line = doc->getLineObject(i);
        // // Ако даден ред в оригиналния документ е подвижен...
        if (line != nullptr && line->isMovable()) {
            // // ...заместваме текста му със следващия по ред от сортирания списък.
            doc->setLineText(i, movableLines[movable_idx].text);
            movable_idx++;
        }
        // // Неподвижните редове се пропускат и остават на местата си.
    }
}

// // Логиката за отмяна на командата е идентична с тази на предишните команди.
// // Тя изтрива текущото състояние и го възстановява напълно от резервното копие.
void SortCommand::undo() {
    // // Изтриваме напълно текущото съдържание на документа.
    size_t currentLineCount = doc->getLineCount();
    for (size_t i = 0; i < currentLineCount; ++i) {
        doc->removeLine(0);
    }

    // // Възстановяваме документа от нашето резервно копие.
    for (size_t i = 0; i < originalLines.size(); ++i) {
        doc->insertLine(i, originalLines[i]);
    }
}

void SortCommand::setDocument(TextDocument* newDoc) {
    this->doc = newDoc;
}