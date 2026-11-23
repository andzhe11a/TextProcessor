#include "SetLineTextCommand.h"
#include "TextDocument.h"

// Имплементация на конструктора.
// Приемаме документа, индекса и новия текст.
// Променливата за стария текст се инициализира като празна,
// тъй като ще я попълним по време на изпълнението на execute().
SetLineTextCommand::SetLineTextCommand(TextDocument* doc, size_t index, const std::string& text)
    : doc(doc), lineIndex(index), newText(text), oldText("") {}

void SetLineTextCommand::execute() {
    // 1. ЗАПАЗВАНЕ НА СТАРОТО СЪСТОЯНИЕ: Преди да променим каквото и да е,
    // взимаме текущия текст на реда от документа и го запазваме в 'oldText'.
    this->oldText = doc->getLineText(lineIndex);

    // 2. ИЗПЪЛНЕНИЕ НА ПРОМЯНАТА: След като сме си осигурили "резервно копие",
    // извикваме метода на документа, който заменя текста на реда с новия.
    doc->setLineText(lineIndex, newText);
}

void SetLineTextCommand::undo() {
    // За да отменим промяната, просто извикваме същия метод setLineText,
    // но този път му подаваме стария текст, който запазихме по време на execute().
    // Това ефективно връща реда в предишното му състояние.
    doc->setLineText(lineIndex, oldText);
}

void SetLineTextCommand::setDocument(TextDocument* newDoc) {
    this->doc = newDoc;
}