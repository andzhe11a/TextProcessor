#pragma once
#include "Command.h"
#include <string>
#include <vector>

class TextDocument;

// // Конкретна команда за разбъркване на редовете в документа.
// // Командата е "умна" и разбърква само редовете, които са маркирани като 'подвижни' (movable),
// // като оставя неподвижните (напр. номерираните) на техните оригинални места.
class ShuffleCommand : public Command {
public:
    // // Конструктор, който приема указател към документа, който ще бъде разбъркан.
    ShuffleCommand(TextDocument* doc);

    void execute() override;
    void undo() override;
    void setDocument(TextDocument* doc) override;
private:
    // // Указател към документа, който ще бъде модифициран.
    TextDocument* doc;

    // // Вектор за "резервно копие" на целия документ преди промяната,
    // // нужен за 'undo' операцията.
    std::vector<std::string> originalLines;
};