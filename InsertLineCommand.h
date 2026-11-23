#pragma once
#include "Command.h"
#include <string>

// В хедър файла (.h) ми е нужен само указател към TextDocument (TextDocument*).
// За това е достатъчно само да 'обещая' на компилатора, че такъв клас съществува,
// което се прави с class TextDocument;. Така избягвам да включвам целия голям хедър
// на TextDocument във всеки хедър на команда, което значително ускорява компилацията.
// Пълната дефиниция на класа TextDocument ми е нужна чак в .cpp файла,
// където реално викам неговите методи (doc->insertLine()), и затова там използвам #include."
class TextDocument;

// Конкретна команда за вмъкване на ред в документ.
// Този клас капсулира цялата информация, необходима за
// извършването и отмяната на операцията по вмъкване на ред.
class InsertLineCommand : public Command {
public:
    // Конструктор, който приема всичко необходимо за изпълнение на командата:
    // - Указател към документа, който ще бъде променен (получателят на командата).
    // - Индексът, на който ще се вмъкне редът.
    // - Текстът, който ще бъде вмъкнат.
    InsertLineCommand(TextDocument* doc, size_t index, const std::string& text);

    void execute() override;
    void undo() override;
    void setDocument(TextDocument* doc) override;
private:
    TextDocument* doc;        // Указател към документа, върху който оперираме.
    size_t lineIndex;         // Мястото (индексът), където се извършва вмъкването.
    std::string textToInsert; // Текстът, който се вмъква.
};
