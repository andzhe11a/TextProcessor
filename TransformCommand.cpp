#include "TransformCommand.h"
#include "TextDocument.h"

char charToUpper(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - ('a' - 'A');
    }
    return c;
}

char charToLower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

// Конструкторът запазва указателя към документа и типа на трансформацията.
TransformCommand::TransformCommand(TextDocument* doc, Type type)
    : doc(doc), transformType(type) {}

void TransformCommand::execute() {
    // // Етап 1: Правим пълно резервно копие на документа в 'originalLines'.
    originalLines.clear();
    size_t lineCount = doc->getLineCount();
    for (size_t i = 0; i < lineCount; ++i) {
        originalLines.push_back(doc->getLineText(i));
    }

    // // Етап 2: Прилагаме трансформацията ред по ред.
    for (size_t i = 0; i < lineCount; ++i) {
        // // ВАЖНО: Взимаме целия Line обект, за да можем да проверим свойствата му.
        BaseLine* line_obj = doc->getLineObject(i);

        // // Проверяваме дали редът съществува и дали той ПОЗВОЛЯВА да бъде трансформиран.
        // // Това е ключов момент от полиморфния дизайн! QuotedLine ще върне 'false'
        // // и неговият текст ще бъде пропуснат, точно както се изисква.
        if (line_obj != nullptr && line_obj->isTransformable()) {
            std::string oldText = line_obj->getText();
            std::string newText = ""; // Подготвяме празен низ за новия текст.
            // // Обхождаме стария текст символ по символ.
            for (char c : oldText) {
                // // В зависимост от типа на командата, прилагаме правилната трансформация.
                if (transformType == Type::TO_UPPER) {
                    newText += charToUpper(c);
                } else {
                    newText += charToLower(c);
                }
            }
            // // Променяме реда в документа само ако реално е настъпила промяна.
            if (oldText != newText) {
                doc->setLineText(i, newText);
            }
        }
    }
}

// // Логиката за отмяна на командата е идентична с тази на TrimCommand.
// // Тя изтрива текущото състояние и го възстановява напълно от резервното копие.
void TransformCommand::undo() {
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

void TransformCommand::setDocument(TextDocument* newDoc) {
    this->doc = newDoc;
}