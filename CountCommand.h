#pragma once
#include "Command.h"
#include <string>

class TextDocument;

class CountCommand : public Command {
public:
    CountCommand(TextDocument* doc, const std::string& word);

    void execute() override;
    void undo() override;
private:
    TextDocument* doc;
    std::string wordToCount;
};