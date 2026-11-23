#include "RemoveLineCommand.h"
#include "TextDocument.h"

// Приемаме документа и индекса, а променливата за изтрития текст
// се инициализира като празна. Тя ще бъде попълнена по-късно.
RemoveLineCommand::RemoveLineCommand(TextDocument* doc, size_t index)
    : doc(doc), lineIndex(index), removedText("") {}

void RemoveLineCommand::execute() {
    // 1. ЗАПАЗВАНЕ: Преди да изтрием реда, първо взимаме текста му от документа
    // и го запазваме в нашата член-променлива 'removedText'.
    this->removedText = doc->getLineText(lineIndex);

    // 2. ИЗПЪЛНЕНИЕ: След като сме запазили текста за 'undo',
    // извикваме метода на документа, който реално премахва реда.
    doc->removeLine(lineIndex);
}

void RemoveLineCommand::undo() {
    // За да отменим изтриване на ред, просто трябва да го вмъкнем обратно
    // на същата позиция, използвайки текста, който запазихме по време на execute().
    doc->insertLine(lineIndex, removedText);
}

void RemoveLineCommand::setDocument(TextDocument* newDoc) {
    this->doc = newDoc;
}