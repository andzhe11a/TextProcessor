#pragma once
#include <string>

class BaseLine {
public:
    // Декларация на конструктора на класа, който създава обект от тип BaseLine.
    BaseLine(const std::string& lineText);

    // Изрично искаме деструктора, генериран от компилатора.
    // Той е напълно достатъчен, тъй като член-променливите (std::string)
    // знаят как да се самопочистят и не е нужна допълнителна логика.
    virtual ~BaseLine() = default;

    std::string getText() const;
    void setText(const std::string& newText);

    // Виртуални функции, които позволяват на програмата да "пита" реда за неговите
    // специфични свойства, без да знае точния му тип. Наследниците могат да
    // предефинират (override) тези функции, за да променят поведението си.
    virtual bool isMovable() const;
    virtual bool isTransformable() const;
    virtual bool isNumeric() const;

protected:
    // Член-променлива от тип std::string, която съхранява самия текст на реда.
    std::string text;
};