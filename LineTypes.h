#pragma once
#include "BaseLine.h"
#include <string>

// Декларация на "фабрична функция". Нейната задача е да анализира текстов ред
// и да създаде обект от най-подходящия наследяващ клас по-долу.
BaseLine* createLine(const std::string& rawLine);

// Представлява обикновен текстов ред без специални свойства.
// Наследява цялото поведение по подразбиране от BaseLine.
class StandardLine : public BaseLine {
public:
    StandardLine(const std::string& lineText);
};

// Представлява номериран ред (напр. "1. ...").
// Не може да бъде преместван при операции за сортиране.
class NumberedLine : public BaseLine {
public:
    NumberedLine(const std::string& lineText);
    // Предефинира базовия метод, за да забрани преместването.
    bool isMovable() const override;
};


// Представлява ред, ограден в кавички (напр. "some text").
// Не може да бъде трансформиран (напр. към главни/малки букви).
class QuotedLine : public BaseLine {
public:
    QuotedLine(const std::string& lineText);
    // Предефинира базовия метод, за да забрани трансформации.
    bool isTransformable() const override;
};


// Представлява ред, който съдържа само число (напр. "123").
// Сравнява се по числена стойност, а не лексикографски.
class NumericLine : public BaseLine {
public:
    NumericLine(const std::string& lineText);
    // Предефинира базовия метод, за да го идентифицира като числов.
    bool isNumeric() const override;
};