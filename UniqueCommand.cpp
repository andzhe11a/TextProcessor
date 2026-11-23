#include "UniqueCommand.h"
#include "TextDocument.h"

// // Помощна функция, която извършва проста проверка дали даден низ се съдържа във вектор от низове.
// // Използва се, за да следим кои редове вече сме "видели".
bool contains(const std::vector<std::string>& vec, const std::string& value) {
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == value) {
            return true;
        }
    }
    return false;
}

UniqueCommand::UniqueCommand(TextDocument* doc) : doc(doc) {}

// // Логиката за изпълнение на командата.
// // 1. Прави резервно копие на оригиналния документ.
// // 2. Филтрира редовете, като запазва само уникалните в нов списък.
// // 3. Заменя съдържанието на документа с филтрирания списък.
// //
void UniqueCommand::execute() {
    // // Етап 1: Правим пълно резервно копие на документа в 'originalLines'.
    originalLines.clear();
    size_t lineCount = doc->getLineCount();
    for (size_t i = 0; i < lineCount; ++i) {
        originalLines.push_back(doc->getLineText(i));
    }

    // // Етап 2: Филтрираме и намираме уникалните редове.
    // // 'uniqueLines' ще съдържа крайния резултат.
    std::vector<std::string> uniqueLines;
    // // 'seenLines' ще използваме, за да следим кои редове вече сме добавили.
    std::vector<std::string> seenLines;

    // // Обхождаме ОРИГИНАЛНИТЕ редове от нашето копие.
    for (size_t i = 0; i < originalLines.size(); ++i) {
        const std::string& currentText = originalLines[i];
        // // Ако НЕ сме виждали този ред досега...
        if (!contains(seenLines, currentText)) {
            // // ...го добавяме към списъка с уникални редове...
            uniqueLines.push_back(currentText);
            // // ...и го маркираме като "видян".
            seenLines.push_back(currentText);
        }
    }

    // // Етап 3: Обновяваме документа с филтрирания списък.
    // // Първо изтриваме всичко от текущия документ.
    for (size_t i = 0; i < lineCount; ++i) {
        doc->removeLine(0);
    }
    // // След това добавяме само уникалните редове.
    for (size_t i = 0; i < uniqueLines.size(); ++i) {
        doc->insertLine(i, uniqueLines[i]);
    }
}

// // Логиката за отмяна на командата е идентична с тази на предишните команди.
// // Тя изтрива текущото състояние и го възстановява напълно от резервното копие.
void UniqueCommand::undo() {
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

void UniqueCommand::setDocument(TextDocument* newDoc) {
    this->doc = newDoc;
}