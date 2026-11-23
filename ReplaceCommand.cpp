#include "ReplaceCommand.h"
#include "TextDocument.h"

ReplaceCommand::ReplaceCommand(TextDocument* doc, std::string find, std::string replace)
    : doc(doc), findStr(find), replaceStr(replace) {}

void ReplaceCommand::execute() {
    // // Етап 1: Правим пълно резервно копие на документа в 'originalLines'.
    originalLines.clear();
    size_t lineCount = doc->getLineCount();
    for (size_t i = 0; i < lineCount; ++i) {
        originalLines.push_back(doc->getLineText(i));
    }

    // // Етап 2: Обхождаме документа ред по ред и извършваме замяната.
    for (size_t i = 0; i < lineCount; ++i) {
        std::string currentText = doc->getLineText(i);
        std::string originalText = currentText; // Запазваме копие за сравнение накрая.

        // // Цикъл, който намира и замества всички срещания на 'findStr' в текущия ред.
        size_t pos = currentText.find(findStr, 0); // Търсим първото срещане от началото.
        while (pos != std::string::npos) { // Цикълът върти, докато 'find' намира нещо.
            // // Заместваме намерения текст.
            currentText.replace(pos, findStr.length(), replaceStr);
            // // Търсим следващото срещане, КАТО ЗАПОЧВАМЕ СЛЕД ЗАМЕНЕНИЯ ТЕКСТ.
            // // Това е много важно, за да се избегне безкраен цикъл (напр. при замяна на "а" с "аа").
            pos = currentText.find(findStr, pos + replaceStr.length());
        }

        // // Променяме реда в документа само ако реално е настъпила промяна.
        if (originalText != currentText) {
            doc->setLineText(i, currentText);
        }
    }
}

void ReplaceCommand::undo() {
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

void ReplaceCommand::setDocument(TextDocument* newDoc) {
    this->doc = newDoc;
}