#include "CountCommand.h"
#include "TextDocument.h"
#include <iostream>

CountCommand::CountCommand(TextDocument* doc, const std::string& word)
    : doc(doc), wordToCount(word) {}

void CountCommand::execute() {
    if (doc == nullptr || wordToCount.empty()) {
        std::cout << "Cannot execute count: no active document or word to count." << std::endl;
        return;
    }

    size_t totalLines = 0;
    size_t lineCount = doc->getLineCount();

    for (size_t i = 0; i < lineCount; ++i) {
        std::string line = doc->getLineText(i);
        size_t pos = line.find(wordToCount, 0);

        if (pos != std::string::npos) {
            lineCount++;
            pos = line.find(wordToCount, pos + 1);
        }
    }
    std::cout << "The word '" << wordToCount << "' appears " << totalCount << " times." << std::endl;
}

void CountCommand::undo() {}