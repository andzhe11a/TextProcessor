#pragma once
#include "Command.h"
#include <string>
#include <vector>

class TextDocument;

// Конкретна команда за прилагане на текстови трансформации (към главни/малки букви).
// Класът използва 'enum class', за да може една и съща команда да изпълнява
// различни, но сходни операции.
class TransformCommand : public Command {
public:
    // Тип изброяване (enum class), който дефинира възможните видове трансформации.
    // Това ни позволява да избегнем създаването на отделни класове ToUpperCommand и ToLowerCommand.
    enum class Type { TO_UPPER, TO_LOWER };

    // // Конструктор, който приема указател към документа и типа на трансформацията.
    TransformCommand(TextDocument* doc, Type type);

    void execute() override;
    void undo() override;
    void setDocument(TextDocument* doc) override;
private:
    // Указател към документа, който ще бъде модифициран.
    TextDocument* doc;
    // Съхранява конкретния тип трансформация, която трябва да се изпълни.
    Type transformType;

    // Вектор за "резервно копие" на целия документ преди промяната.
    std::vector<std::string> originalLines;
};