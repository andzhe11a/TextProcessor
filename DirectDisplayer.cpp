#include <iostream>
#include "DirectDisplayer.h"
#include "TextDocument.h"

void DirectDisplayer::display(const TextDocument& doc) const {
    for (size_t i = 0; i < doc.getLineCount(); ++i) {
        std::cout << doc.getLineText(i) << std::endl;
    }
}