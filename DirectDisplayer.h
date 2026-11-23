#pragma once
#include "Displayer.h"

class TextDocument;

class DirectDisplayer : public Displayer {
public:
    void display(const TextDocument& doc) const override;
};