#pragma once
#include <iostream>
#include "Displayer.h"

class TextDocument;

// // Конкретна "стратегия" за извеждане на текст, която "обгръща" (wraps)
// // дългите редове, като ги разделя на няколко по-къси реда с
// // предварително зададена максимална ширина.
class WrapDisplayer : public Displayer {
public:
    WrapDisplayer(size_t wrapWidth);

    void display(const TextDocument& doc) const override;
private:
    size_t width;
};