// Файл: OnBlockCommand.cpp
#include "OnBlockCommand.h"
#include "TextDocument.h"

OnBlockCommand::OnBlockCommand(TextDocument* doc, size_t from, size_t to, Command* subCommand)
    : doc(doc), from(from), to(to), subCommand(subCommand) {}

OnBlockCommand::~OnBlockCommand() {
    // // Тъй като OnBlockCommand "притежава" под-командата, той е отговорен да я изтрие.
    delete subCommand;
}

void OnBlockCommand::execute() {
    // 1. Правим резервно копие на оригиналния блок (както преди)
    originalBlockLines.clear();
    for (size_t i = from; i <= to; ++i) {
        originalBlockLines.push_back(doc->getLineText(i));
    }

    // 2. Създаваме временен документ
    TextDocument tempDoc("temp_for_block");
    while(tempDoc.getLineCount() > 0) { tempDoc.removeLine(0); }
    for (const auto& line : originalBlockLines) {
        tempDoc.insertLine(tempDoc.getLineCount(), line);
    }

    // 3. ПРЕНАСОЧВАМЕ под-командата да работи върху временния документ.
    subCommand->setDocument(&tempDoc);

    // 4. ИЗПЪЛНЯВАМЕ я. Сега тя ще промени САМО tempDoc.
    subCommand->execute();

    // 5. ВРЪЩАМЕ под-командата да сочи отново към оригиналния документ.
    // Това е важно, в случай че обектът се използва отново (напр. в redo).
    subCommand->setDocument(doc);

    // 6. Запазваме резултата от tempDoc (както преди)
    newBlockLines.clear();
    for(size_t i = 0; i < tempDoc.getLineCount(); ++i) {
        newBlockLines.push_back(tempDoc.getLineText(i));
    }

    // 7. Заменяме блока в основния документ (както преди)
    for (size_t i = 0; i < originalBlockLines.size(); ++i) {
        doc->removeLine(from);
    }
    for (size_t i = 0; i < newBlockLines.size(); ++i) {
        doc->insertLine(from + i, newBlockLines[i]);
    }
}

void OnBlockCommand::undo() {
    // // Логиката за undo е да направим обратното на стъпка 5 от execute().

    // // 1. ИЗТРИВАМЕ МОДИФИЦИРАНИЯ БЛОК, който сме вмъкнали
    // // Знаем колко реда са, защото сме ги запазили в newBlockLines.
    for (size_t i = 0; i < newBlockLines.size(); ++i) {
        doc->removeLine(from);
    }

    // // 2. ВРЪЩАМЕ ОРИГИНАЛНИЯ БЛОК ОТ РЕЗЕРВНОТО КОПИЕ
    for (size_t i = 0; i < originalBlockLines.size(); ++i) {
        doc->insertLine(from + i, originalBlockLines[i]);
    }
}