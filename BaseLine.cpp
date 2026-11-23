#include "BaseLine.h"

// Използва инициализиращ списък (: text(lineText)) за максимална ефективност,
// като директно конструира член-променливата text със стойността на подадения низ.
BaseLine::BaseLine(const std::string& lineText) : text(lineText) {}

std::string BaseLine::getText() const {
    return text;
}

void BaseLine::setText(const std::string& newText) {
    text = newText;
}

// Тези функции дефинират поведението по подразбиране за един "стандартен" ред.
// Наследяващите класове, които искат да имат по-специфично поведение
// (напр. NumberedLine не трябва да се мести), ще предефинират (override)
// съответния метод, за да променят тази логика.

bool BaseLine::isMovable() const {
    return true;
}

bool BaseLine::isTransformable() const {
    return true;
}

bool BaseLine::isNumeric() const {
    return false;
}