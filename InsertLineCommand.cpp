#include "InsertLineCommand.h"
#include "TextDocument.h"

InsertLineCommand::InsertLineCommand(TextDocument* doc, size_t index, const std::string& text)
    : doc(doc), lineIndex(index), textToInsert(text) {}

void InsertLineCommand::execute() {
    // Просто извикваме съответния метод на документа, като му подаваме
    // данните, които командата пази в себе си.
    doc->insertLine(lineIndex, textToInsert);
}

void InsertLineCommand::undo() {
    // За да отменим вмъкване на ред на даден индекс, просто трябва
    // да премахнем реда, намиращ се на същия този индекс.
    doc->removeLine(lineIndex);
}

void InsertLineCommand::setDocument(TextDocument* newDoc) {
    this->doc = newDoc;
}