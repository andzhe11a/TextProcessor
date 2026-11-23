// Файл: OnBlockCommand.h
#pragma once
#include "Command.h"
#include <vector>
#include <string>

class TextDocument;

// //
// // "Мета-команда", която изпълнява друга команда, но само върху част (блок) от документа.
// // Тя е напълно undo-able, като пази копие на оригиналния блок.
// //
class OnBlockCommand : public Command {
public:
    // // Конструктор, който приема всичко необходимо: документа, обхвата на блока
    // // и под-командата, която да бъде изпълнена.
    OnBlockCommand(TextDocument* doc, size_t from, size_t to, Command* subCommand);
    // // Деструкторът трябва да изтрие под-командата, тъй като този клас поема собственост над нея.
    ~OnBlockCommand() override;

    void execute() override;
    void undo() override;
private:
    TextDocument* doc;
    size_t from;
    size_t to;
    Command* subCommand; // "Вътрешната" команда (напр. SortCommand)

    // // Тук ще пазим резервно копие САМО на оригиналния блок.
    std::vector<std::string> originalBlockLines;
    // // Тук ще пазим копие на променения блок, за да знаем колко реда да изтрием при undo.
    std::vector<std::string> newBlockLines;
};