#include "MacroCommand.h"

MacroCommand::MacroCommand(const std::string& macroName) : name(macroName) {}

// // Тъй като MacroCommand "притежава" командите, които са му добавени,
// // негова отговорност е да освободи паметта им, когато самият той бъде унищожен.
MacroCommand::~MacroCommand() {
    // // Обхождаме вектора и извикваме 'delete' за всеки указател към команда.
    for (Command* cmd : commands) {
        delete cmd;
    }
    // // Векторът сам ще се почисти след това.
}


// // Добавя указател към нова команда в края на вътрешния вектор.
void MacroCommand::addCommand(Command* cmd) {
    commands.push_back(cmd);
}

const std::string& MacroCommand::getName() const {
    return name;
}

void MacroCommand::execute() {
    // // Просто обхождаме списъка с команди от началото към края...
    for (Command* cmd : commands) {
        // // ...и извикваме 'execute()' на всяка една от тях.
        cmd->execute();
    }
}

void MacroCommand::undo() {
    // // За да отменим поредица от команди, трябва да го направим в ОБРАТЕН РЕД.
    // // Затова обхождаме вектора отзад напред (от последния към първия елемент).
    for (size_t i = commands.size(); i > 0; --i) {
        // // И извикваме 'undo()' на всяка команда.
        // // Така, ако сме имали [insert, remove, edit], ще се изпълни undo на edit,
        // // след това undo на remove и накрая undo на insert.
        commands[i - 1]->undo();
    }
}

void MacroCommand::setDocument(TextDocument* doc) {
    // // Обхождаме всички команди в нашия вътрешен списък...
    for (Command* cmd : commands) {
        // // ...и извикваме техния собствен setDocument метод.
        cmd->setDocument(doc);
    }
}