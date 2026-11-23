#pragma once
#include "Command.h"
#include <string>
#include <vector>

class TextDocument;

// // Конкретна команда за търсене и замяна на текст в целия документ.
// // Както и другите команди, които работят върху целия документ, и тази
// // пази пълно резервно копие на оригиналните редове за 'undo' функционалността.
class ReplaceCommand : public Command {
public:
    // // Конструктор, който приема указател към документа, низа, който
    // // ще се търси, и низа, с който ще се замества.
    ReplaceCommand(TextDocument* doc, std::string find, std::string replace);

    void execute() override;
    void undo() override;
    void setDocument(TextDocument* doc) override;
private:
    // // Указател към документа, който ще бъде модифициран.
    TextDocument* doc;
    // // Низът, който ще търсим.
    std::string findStr;
    // // Низът, с който ще заместваме.
    std::string replaceStr;

    // // Вектор за "резервно копие" на целия документ преди промяната.
    std::vector<std::string> originalLines;
};