#include "ShuffleCommand.h"
#include "TextDocument.h"

ShuffleCommand::ShuffleCommand(TextDocument* doc) : doc(doc) {}

// // Логиката за изпълнение на командата. Процесът е разделен на няколко стъпки:
// // 1. Прави се пълно резервно копие на документа.
// // 2. В отделен списък се извличат само редовете, които могат да се местят.
// // 3. Този отделен списък се разбърква.
// // 4. Променените редове се връщат обратно в основния документ.
void ShuffleCommand::execute() {
    // // Етап 1: Правим пълно резервно копие на документа в 'originalLines'.
    originalLines.clear();
    size_t lineCount = doc->getLineCount();
    for (size_t i = 0; i < lineCount; ++i) {
        originalLines.push_back(doc->getLineText(i));
    }

    // // Етап 2: Извличаме само подвижните редове.
    std::vector<std::string> movableLinesText;
    for (size_t i = 0; i < lineCount; ++i) {
        BaseLine* line = doc->getLineObject(i);
        // // Използваме полиморфния метод isMovable(), за да филтрираме редовете.
        // // NumberedLine ще върне 'false' и няма да бъде добавен в списъка за разбъркване.
        if (line != nullptr && line->isMovable()) {
            movableLinesText.push_back(line->getText());
        }
    }

    // // Етап 3: Разбъркваме списъка с подвижните редове.
    // // Това е имплементация на класическия алгоритъм за разбъркване Fisher-Yates.
    for (size_t i = movableLinesText.size() - 1; i > 0; --i) {
        size_t j = rand() % (i + 1); // Избираме случаен индекс от 0 до i.
        // // Разменяме елементите на позиции i и j.
        std::string temp = movableLinesText[i];
        movableLinesText[i] = movableLinesText[j];
        movableLinesText[j] = temp;
    }

    // // Етап 4: Връщаме разбърканите редове обратно в документа.
    int movable_idx = 0; // Брояч за позицията в разбъркания списък.
    // // Обхождаме ОРИГИНАЛНИЯ документ ред по ред.
    for (size_t i = 0; i < lineCount; ++i) {
        BaseLine* line = doc->getLineObject(i);
        // // Ако даден ред е подвижен...
        if (line != nullptr && line->isMovable()) {
            // // ...заместваме текста му със следващия по ред от разбъркания списък.
            doc->setLineText(i, movableLinesText[movable_idx]);
            movable_idx++;
        }
        // // Ако редът не е подвижен, просто го пропускаме и той си остава на мястото.
    }
}

// // Логиката за отмяна на командата е идентична с тази на предишните команди.
// // Тя изтрива текущото състояние и го възстановява напълно от резервното копие.
void ShuffleCommand::undo() {
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

void ShuffleCommand::setDocument(TextDocument* newDoc) {
    this->doc = newDoc;
}