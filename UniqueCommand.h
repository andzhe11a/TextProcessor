#pragma once
#include "Command.h"
#include <string>
#include <vector>

class TextDocument;

// // Конкретна команда за премахване на дублиращи се редове от документа.
// // Запазва само първото срещане на всеки уникален ред и поддържа реда им.
class UniqueCommand : public Command {
public:
    UniqueCommand(TextDocument* doc);

    void execute() override;
    void undo() override;
    void setDocument(TextDocument* doc) override;

private:
    // // Указател към документа, който ще бъде модифициран.
    TextDocument* doc;

    // // Вектор за "резервно копие" на целия документ преди премахването на дубликатите.
    // // Нужен е за 'undo' операцията.
    std::vector<std::string> originalLines;
};