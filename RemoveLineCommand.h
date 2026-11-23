#pragma once
#include "Command.h"
#include <string>

class TextDocument;

// Конкретна команда за премахване на ред от документ.
// Класът съхранява индекса на реда, който трябва да бъде премахнат,
// и запазва изтрития текст, за да може да го възстанови при операция 'undo'.
class RemoveLineCommand : public Command {
public:
    // Конструктор, който приема получателя (документа) и индекса на реда за триене.
    RemoveLineCommand(TextDocument* doc, size_t index);

    void execute() override;
    void undo() override;
    void setDocument(TextDocument* doc) override;
private:
    TextDocument* doc;        // Указател към документа, върху който оперираме.
    size_t lineIndex;         // Индексът на реда, който ще бъде изтрит.

    // Променлива, в която ще запазим текста на изтрития ред.
    // Това е ключово за работата на undo().
    std::string removedText;
};