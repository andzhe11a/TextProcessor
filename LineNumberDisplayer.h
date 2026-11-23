// Файл: LineNumberDisplayer.h
#pragma once
#include "Displayer.h"

// Forward declaration
class TextDocument;

// //
// // Конкретна "стратегия" за извеждане, която добавя номер пред всеки ред.
// //
class LineNumberDisplayer : public Displayer {
public:
    // // Предефинираме метода от базовия клас Displayer.
    // // Тази стратегия е "stateless" - няма нужда от член-променливи
    // // или специален конструктор.
    void display(const TextDocument& doc) const override;
};