#pragma once
#include "Command.h"
#include <string>

class TextDocument;

// Конкретна команда за промяна на текста на съществуващ ред.
// Класът капсулира новия текст, който трябва да се запише,
// и запазва стария текст на реда, за да може да възстанови предишното
// състояние при операция 'undo'.
class SetLineTextCommand : public Command {
public:
    // Конструктор, който приема получателя (документа), индекса на реда за
    // промяна и новия текст, който да бъде записан.
    SetLineTextCommand(TextDocument* doc, size_t index, const std::string& text);

    void execute() override;
    void undo() override;
    void setDocument(TextDocument* doc) override;
private:
    TextDocument* doc;    // Указател към документа, върху който оперираме.
    size_t lineIndex;     // Индексът на реда, който ще бъде променен.
    std::string newText;  // Новият текст, който ще бъде записан при execute().
    std::string oldText;  // Тук ще запазим стария текст за нуждите на undo().
};