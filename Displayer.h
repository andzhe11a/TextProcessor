#pragma once

class TextDocument;

class Displayer {
public:
    virtual ~Displayer() = default;

    virtual void display(const TextDocument& doc) const = 0;
};