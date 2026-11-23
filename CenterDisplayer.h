#pragma once
#include <iostream>
#include "Displayer.h"

class TextDocument;

class CenterDisplayer : public Displayer {
public:
    // // Конструктор, който приема ширината, спрямо която ще се центрира текстът.
    // // Това прави стратегията гъвкава и конфигурируема.
    CenterDisplayer(size_t centerWidth);

    void display(const TextDocument& doc) const override;
private:
    // // Съхранява ширината за центриране, зададена в конструктора.
    size_t width;
};
