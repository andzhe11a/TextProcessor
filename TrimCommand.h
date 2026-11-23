#pragma once
#include "Command.h"
#include <string>
#include <vector>

class TextDocument;

// Конкретна команда за премахване на водещи/завършващи празни символи от всички редове.
// Тъй като тази операция може да промени целия документ, командата трябва да
// запази копие на всички оригинални редове, за да може да ги възстанови при 'undo'.
class TrimCommand : public Command {
public:
    // Конструктор, който приема само указател към документа, върху който ще се работи.
    TrimCommand(TextDocument* doc);

    void execute() override;
    void undo() override;
    void setDocument(TextDocument* doc) override;

private:
    // Указател към документа, който ще бъде модифициран.
    TextDocument* doc;

    // Вектор, в който ще направим "резервно копие" на целия документ
    // преди да го променим. Това е ключово за работата на undo().
    std::vector<std::string> originalLines;
};