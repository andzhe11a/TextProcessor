#pragma once
#include "Command.h"
#include <string>
#include <vector>

class TextDocument;

// // "Композитна" команда, която може да съдържа и изпълнява поредица от други команди.
// // Това е имплементация на 'Composite' дизайн патерна. Позволява ни да третираме
// // една сложна поредица от операции по същия начин, по който третираме една проста команда.
class MacroCommand : public Command {
public:
    MacroCommand(const std::string& macroName);

    ~MacroCommand() override;

    void addCommand(Command* cmd);

    const std::string& getName() const;

    void execute() override;
    void undo() override;
    void setDocument(TextDocument* doc) override;
private:
    // // Име на макроса, за да може да бъде идентифициран и извикан по-късно.
    std::string name;

    // // Колекция от указатели към командите, които съставляват този макрос.
    std::vector<Command*> commands;
};