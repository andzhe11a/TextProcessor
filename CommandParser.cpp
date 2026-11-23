#include "CommandParser.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include "SetLineTextCommand.h"
#include "InsertLineCommand.h"
#include "RemoveLineCommand.h"
#include "TransformCommand.h"
#include "ReplaceCommand.h"
#include "SortCommand.h"
#include "ShuffleCommand.h"
#include "UniqueCommand.h"
#include "TrimCommand.h"
#include "Command.h"
#include "CountCommand.h"

Command* CommandParser::parse(const std::string& input, TextDocument* doc) {
    // // Първоначална проверка за валидност. Ако няма вход или документ, не можем да продължим.
    if (input.empty() || doc == nullptr) {
        return nullptr;
    }

    // // --- Разделяне на входа на думи (аргументи) ---
    // // Тук ръчно разделяме входния низ по интервали, за да получим списък от аргументи.
    // // Например, "edit 5 new text" ще стане вектор ["edit", "5", "new", "text"].
    std::vector<std::string> args;
    std::string current_arg;
    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] == ' ') {
            if (!current_arg.empty()) {
                args.push_back(current_arg);
                current_arg = "";
            }
        } else {
            current_arg += input[i];
        }
    }
    if (!current_arg.empty()) {
        args.push_back(current_arg);
    }
    // // Ако след разделянето нямаме никакви аргументи (напр. входът е бил само интервали), връщаме nullptr.
    if (args.empty()) return nullptr;

    // // --- Разпознаване и създаване на команда ---
    // // Първата дума винаги е името на самата команда.
    std::string commandWord = args[0];

    // // Поредица от проверки, за да видим коя команда съвпада.
    // // За команди без допълнителни параметри, просто създаваме нов обект.
    if (commandWord == "sort") { return new SortCommand(doc); }
    if (commandWord == "shuffle") { return new ShuffleCommand(doc); }
    if (commandWord == "unique") { return new UniqueCommand(doc); }
    if (commandWord == "trim") { return new TrimCommand(doc); }

    // // За команди с параметри, проверяваме и броя на аргументите.
    if (commandWord == "transform" && args.size() >= 2) {
        TransformCommand::Type type;
        if (args[1] == "toupper") { type = TransformCommand::Type::TO_UPPER; }
        else if (args[1] == "tolower") { type = TransformCommand::Type::TO_LOWER; }
        else { return nullptr; } // Ако вторият параметър е невалиден.
        return new TransformCommand(doc, type);
    }
    if (commandWord == "replace" && args.size() >= 3) {
        return new ReplaceCommand(doc, args[1], args[2]);
    }

    // // За команди, които очакват номер на ред, използваме try-catch блок.
    // // Това е за "защита" от случаи, в които потребителят въведе текст вместо число.
    if (commandWord == "edit" && args.size() >= 3) {
        try {
            // // Преобразуваме втория аргумент от низ в число.
            int lineNum = std::stoi(args[1]);
            // // Сглобяваме останалите аргументи в един общ низ за новия текст.
            std::string newText = "";
            for (size_t i = 2; i < args.size(); ++i) {
                newText += args[i] + (i == args.size() - 1 ? "" : " ");
            }
            // // Създаваме командата, като подаваме 0-базиран индекс (lineNum - 1).
            return new SetLineTextCommand(doc, lineNum - 1, newText);
        } catch (const std::exception& e) {
            // // Ако stoi() хвърли грешка, информираме потребителя и връщаме nullptr.
            std::cout << "Error: Invalid line number provided. Please use a number." << std::endl;
            return nullptr;
        }
    }
    if (commandWord == "insert" && args.size() >= 3) {
        try {
            int lineNum = std::stoi(args[1]);
            std::string newText = "";
            for (size_t i = 2; i < args.size(); ++i) {
                newText += args[i] + (i == args.size() - 1 ? "" : " ");
            }
            // // Потребителите броят от 1, но векторите в C++ - от 0, затова вадим 1.
            return new InsertLineCommand(doc, lineNum - 1, newText);
        } catch (const std::exception& e) {
            std::cout << "Error: Invalid line number provided. Please use a number." << std::endl;
            return nullptr;
        }
    }
    if (commandWord == "remove" && args.size() >= 2) {
        try {
            int lineNum = std::stoi(args[1]);
            return new RemoveLineCommand(doc, lineNum - 1);
        } catch (const std::exception& e) {
            std::cout << "Error: Invalid line number provided. Please use a number." << std::endl;
            return nullptr;
        }
    }
    if (commandWord == "count" && args.size() >= 2) {
        return new CountCommand(doc, args[1]);
    }

    // // Ако нито една от проверките не е била успешна, значи командата е неразпозната.
    return nullptr;
}