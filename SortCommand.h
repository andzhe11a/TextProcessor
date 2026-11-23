#pragma once
#include "Command.h"
#include <string>
#include <vector>

class TextDocument;

// // Конкретна команда за сортиране на редовете в документа.
// // Командата прилага сложна логика, като сортира само "подвижните" редове
// // и сравнява числовите редове по стойност, а останалите - лексикографски.
class SortCommand : public Command {
public:
    SortCommand(TextDocument* doc);

    void execute() override;
    void undo() override;
    void setDocument(TextDocument* doc) override;
private:
    // // Указател към документа, който ще бъде модифициран.
    TextDocument* doc;

    // // Вектор за "резервно копие" на целия документ преди сортирането.
    std::vector<std::string> originalLines;
};