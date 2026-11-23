#include "TextProcessor.h"
#include "TextDocument.h"
#include "CommandParser.h"
#include "Command.h"
#include "OnBlockCommand.h"
#include "MacroCommand.h"
#include "DirectDisplayer.h"
#include "WrapDisplayer.h"
#include "CenterDisplayer.h"
#include "LineNumberDisplayer.h"
#include <iostream>
#include <string>

TextProcessor::TextProcessor() : activeDocument(nullptr), isRunning(true), currentlyRecordingMacro(nullptr) {
    // // Създаваме обектите, които са нужни за работата на процесора.
    // // Задаваме стратегия за извеждане по подразбиране.
    displayer = new DirectDisplayer();
    // // Създаваме парсера, който ще превежда потребителския вход.
    parser = new CommandParser();
    // // Инициализираме генератора на случайни числа. Това е нужно
    // // за команди като ShuffleCommand, за да работят различно всеки път.
    srand(time(nullptr));
}

TextProcessor::~TextProcessor() {
    cleanup();
}

void TextProcessor::clearRedoStack() {
    for (size_t i = 0; i < redoStack.size(); ++i) {
        delete redoStack[i];
    }
    redoStack.clear();
}

// // Помощен метод за освобождаване на ВСЯКА динамично заделена памет.
// // Това е критично важно за избягване на изтичане на памет (memory leaks).
// //
void TextProcessor::cleanup() {
    // // Изтриваме единичните обекти.
    delete displayer;
    delete parser;

    // // Обхождаме и изтриваме всяка команда от undo стека.
    for (size_t i = 0; i < undoStack.size(); ++i) {
        delete undoStack[i];
    }
    undoStack.clear();

    clearRedoStack();
    // // Обхождаме и изтриваме всеки дефиниран макрос.
    for (size_t i = 0; i < macros.size(); ++i) {
        delete macros[i];
    }
    macros.clear();

    // // Изтриваме и макроса, който евентуално се записва в момента.
    delete currentlyRecordingMacro;

    // // Обхождаме и изтриваме всеки отворен документ.
    for (size_t i = 0; i < documents.size(); ++i) {
        delete documents[i];
    }
    documents.clear();
}

// // Централизиран метод за изпълнение на команди.
// // Той решава дали една команда да бъде изпълнена веднага или да бъде добавена към макрос.
// //
void TextProcessor::executeCommand(Command* command) {
    // // Защита срещу невалидни команди (напр. от грешка в парсера).
    if (command == nullptr) return;

    // // Проверяваме дали в момента сме в "режим на запис" на макрос.
    if (currentlyRecordingMacro) {
        // // Ако ДА, командата НЕ се изпълнява.
        // // Вместо това, тя се добавя към списъка с команди на записвания макрос.
        currentlyRecordingMacro->addCommand(command);
        std::cout << "Command added to macro '" << currentlyRecordingMacro->getName() << "'." << std::endl;
    } else {
        // // Ако НЕ, сме в нормален режим на работа.
        // // 1. Изпълняваме командата веднага.
        command->execute();

        clearRedoStack();
        // // 2. Добавяме я в undo стека, за да може да бъде отменена по-късно.
        undoStack.push_back(command);
    }
}

// //
// // Помощен метод, който се грижи за документите с незапазени промени.
// // Извиква се преди затваряне на файл или излизане от програмата.
// // Връща 'true', ако операцията може да продължи (файлът е запазен или потребителят се е съгласил да ги загуби).
// // Връща 'false', ако операцията трябва да бъде прекратена (напр. неуспешен запис).
// //
bool TextProcessor::handleUnsavedChanges(TextDocument* doc) {
    // // Ако няма промени, няма какво да правим. Връщаме 'true', за да продължи операцията.
    if (!doc->hasUnsavedChanges()) {
        return true;
    }
    // // Безкраен цикъл, който се повтаря, докато потребителят не въведе валиден отговор ('y' или 'n').
    while (true) {
        std::cout << "File '" << doc->getFilename() << "' has unsaved changes. Save? (y/n): ";
        std::string answer;
        std::getline(std::cin, answer);

        if (answer == "y") { // // Ако потребителят иска да запази...
            if (doc->save()) { // // ...опитваме се да запишем файла.
                 std::cout << "Saved." << std::endl;
                 return true; // // Ако записът е успешен, продължаваме.
            }
            else { // // Ако записът е неуспешен...
                std::cout << "Error! Could not save file." << std::endl;
                return false; // // ...прекратяваме операцията (напр. 'close').
            }
        } else if (answer == "n") { // // Ако потребителят НЕ иска да запази...
            return true; // // ...продължаваме, като промените ще бъдат загубени.
        } else { // // Ако въведеният отговор е невалиден...
            std::cout << "Invalid input. Please enter 'y' or 'n'." << std::endl;
            // // ...цикълът ще се повтори.
        }
    }
}

// //
// // Метод-разпределител, който обработва всички команди, свързани с файлове.
// //
void TextProcessor::handleFileCommand(const std::string& command, const std::string& args) {
    // // Команда за отваряне на файл.
    if (command == "open") {
        if (args.empty()) { std::cout << "Error: 'open' requires a filename." << std::endl; return; }
        // // Първо проверяваме дали такъв документ вече не е отворен.
        for (size_t i = 0; i < documents.size(); ++i) {
            if (documents[i]->getFilename() == args) {
                // // Ако е отворен, просто го правим активен.
                activeDocument = documents[i];
                std::cout << "Switched to already open document '" << args << "'." << std::endl;
                return;
            }
        }
        // // Ако не е отворен, създаваме нов обект TextDocument...
        TextDocument* newDoc = new TextDocument(args);
        // // ...добавяме го към списъка с отворени документи...
        documents.push_back(newDoc);
        // // ...и го правим активен.
        activeDocument = newDoc;
        std::cout << "Opened new document '" << args << "'." << std::endl;
    }
    // // Команда за информация за отворените файлове.
    else if (command == "info") {
        if (documents.empty()) { std::cout << "No files are open." << std::endl; return; }
        std::cout << "Open Files: " << std::endl;
        // // Обхождаме всички отворени документи и отпечатваме информация за тях.
        for (size_t i = 0; i < documents.size(); ++i) {
            TextDocument* doc = documents[i];
            std::cout << "* " << doc->getFilename();
            if (doc == activeDocument) { std::cout << " (active)"; } // // Отбелязваме активния.
            std::cout << ": " << doc->getLineCount() << " lines, "
                      << doc->getCharacterCount() << " chars ";
            if (doc->hasUnsavedChanges()) { std::cout << "[UNSAVED]"; } // // Отбелязваме, ако има промени.
            std::cout << std::endl;
        }
    }
    // // Команда за запис на активния файл.
    else if (command == "save") {
        if (!activeDocument) { std::cout << "Error: No active document." << std::endl; return; }
        // // Делегираме работата на метода 'save' на самия документ.
        if (activeDocument->save()) {
            std::cout << "Document saved to '" << activeDocument->getFilename() << "'." << std::endl;
        } else {
            std::cout << "Error: Could not save document." << std::endl;
        }
    }
    // // Команда за запис в нов файл.
    else if (command == "saveas") {
        if (!activeDocument) { std::cout << "Error: No active document." << std::endl; return; }
        if (args.empty()) { std::cout << "Error: 'saveas' requires a filename." << std::endl; return; }
        // // Делегираме работата на метода 'saveAs' на самия документ.
        if (activeDocument->saveAs(args)) {
            std::cout << "Document saved as '" << args << "'." << std::endl;
        } else {
            std::cout << "Error: Could not save document." << std::endl;
        }
    }
    // // Команда за смяна на активния файл.
    else if (command == "switch") {
        if (args.empty()) { std::cout << "Error: 'switch' requires a filename." << std::endl; return; }
        bool found = false;
        // // Търсим в списъка с отворени документи.
        for (size_t i = 0; i < documents.size(); ++i) {
            if (documents[i]->getFilename() == args) {
                // // Ако го намерим, просто сменяме указателя 'activeDocument'.
                activeDocument = documents[i];
                std::cout << "Switched to '" << args << "'." << std::endl;
                found = true;
                break;
            }
        }
        if (!found) { std::cout << "Error: Document '" << args << "' is not open." << std::endl; }
    }
    // // Команда за затваряне на активния файл.
    else if (command == "close") {
        if (!activeDocument) { std::cout << "Error: No active document." << std::endl; return; }
        // // Първо извикваме метода, който пита потребителя дали иска да запази промените.
        if (handleUnsavedChanges(activeDocument)) {
            // // Ако методът върне 'true', продължаваме със затварянето.
            std::string closedFilename = activeDocument->getFilename();
            // // Намираме указателя в списъка и го премахваме.
            for (size_t i = 0; i < documents.size(); ++i) {
                if (documents[i] == activeDocument) {
                    documents.erase(documents.begin() + i);
                    break;
                }
            }
            // // Изтриваме самия обект TextDocument, за да освободим паметта.
            delete activeDocument;
            // // Актуализираме 'activeDocument' - или става nullptr, или сочи към първия останал.
            activeDocument = documents.empty() ? nullptr : documents.front();
            std::cout << "Closed '" << closedFilename << "'." << std::endl;
            if (activeDocument) { std::cout << "Switched to '" << activeDocument->getFilename() << "'." << std::endl; }
        } else {
            // // Ако handleUnsavedChanges върне 'false', прекратяваме затварянето.
            std::cout << "Close cancelled." << std::endl;
        }
    }
}

// //
// // Метод-разпределител, който обработва всички команди, свързани с макроси.
// //
void TextProcessor::handleMacroCommand(const std::string& command, const std::string& args) {
    // // Команда за започване на запис на макрос.
    if (command == "macro_begin") {
        // // Проверяваме дали вече не записваме друг макрос.
        if (currentlyRecordingMacro) { std::cout << "Already recording a macro." << std::endl; return; }
        if (args.empty()) { std::cout << "Incorrect arguments. Correct format is: macro_begin <name>" << std::endl; return; }
        // // Създаваме нов обект MacroCommand и го присвояваме на 'currentlyRecordingMacro'.
        // // Това е "флагът", който показва на системата, че сме в режим на запис.
        currentlyRecordingMacro = new MacroCommand(args);
        std::cout << "Started recording macro '" << args << "'." << std::endl;
    }
    // // Команда за край на записа на макрос.
    else if (command == "macro_end") {
        // // Проверяваме дали изобщо сме в режим на запис.
        if (!currentlyRecordingMacro) { std::cout << "Not recording a macro." << std::endl; return; }
        // // Добавяме завършения макрос към общия списък със запазени макроси.
        macros.push_back(currentlyRecordingMacro);
        std::cout << "Finished recording macro '" << currentlyRecordingMacro->getName() << "'." << std::endl;
        // // Нулираме указателя, с което излизаме от режим на запис.
        currentlyRecordingMacro = nullptr;
    }
    // // Команда за изпълнение на вече записан макрос.
    else if (command == "run_macro") {
        if (args.empty()) { std::cout << "Incorrect arguments. Correct format is: run_macro <name>" << std::endl; return; }
        MacroCommand* macroToRun = nullptr;
        // // Търсим макрос с даденото име в списъка със запазени макроси.
        for (size_t i = 0; i < macros.size(); ++i) {
            if (macros[i]->getName() == args) {
                macroToRun = macros[i];
                break;
            }
        }
        // // Ако е намерен, просто извикваме неговия execute() метод.
        if (macroToRun) { std::cout << "Running macro '" << args << "'..." << std::endl; macroToRun->execute(); }
        else { std::cout << "Macro '" << args << "' not found." << std::endl; }
    }
}

// //
// // Метод-разпределител за команди, които управляват самото приложение,
// // а не съдържанието на документа.
// //
void TextProcessor::handleAppCommand(const std::string& command, const std::string& args) {
    // // Команда за изход от програмата.
    if (command == "exit") {
        bool cancelExit = false;
        // // Преди да излезем, трябва да проверим ВСИЧКИ отворени документи за незапазени промени.
        for (size_t i = 0; i < documents.size(); ++i) {
            // // handleUnsavedChanges пита потребителя и връща 'false', ако нещо се обърка (напр. неуспешен запис).
            if (!handleUnsavedChanges(documents[i])) {
                cancelExit = true; // // Ако дори един файл не може да се запази, прекратяваме излизането.
                break;
            }
        }
        // // Ако всичко е наред, задаваме флага 'isRunning' на false, което ще спре основния цикъл на програмата.
        if (!cancelExit) { this->isRunning = false; }
        else { std::cout << "Exit cancelled." << std::endl; }
    }
    // // Команда за извеждане на съдържанието на екрана.
    else if (command == "display") {
        if (!activeDocument) { std::cout << "Error: No active document." << std::endl; return; }
        // // Това е "Strategy" патернът в действие!
        // // Просто викаме 'display' метода на текущия 'displayer', без да се интересуваме
        // // какъв точно е той (Direct, Wrap или Center).
        displayer->display(*activeDocument);
    }
    // // Команда за смяна на текущата стратегия за извеждане.
    else if (command == "set_display") {
        if (args.empty()) { std::cout << "Incorrect arguments. Correct format is:  set_display <direct/wrap/center> [width]" << std::endl; return; }
        // // Задължително изтриваме стария обект-стратегия, за да няма изтичане на памет.
        delete displayer;
        size_t spacePos = args.find(' ');
        if (spacePos != std::string::npos) { // // Ако има втора дума (за ширина)
            std::string displayType = args.substr(0, spacePos);
            int width = std::stoi(args.substr(spacePos + 1));
            // // Създаваме нов обект от правилния тип и го присвояваме на нашия указател.
            if (displayType == "wrap") { displayer = new WrapDisplayer(width); std::cout << "Display set to wrap with width " << width << "." << std::endl; }
            else if (displayType == "center") { displayer = new CenterDisplayer(width); std::cout << "Display set to center with width " << width << "." << std::endl; }
            else { displayer = new DirectDisplayer(); std::cout << "Unknown display type. Changing back to direct." << std::endl; }
        } else {
            // ТУК Е ПРОМЯНАТА
            if (args == "direct") {
                displayer = new DirectDisplayer();
                std::cout << "Display set to direct." << std::endl;
            }
            // ДОБАВЯМЕ НОВА ПРОВЕРКА:
            else if (args == "linenumbers") {
                displayer = new LineNumberDisplayer();
                std::cout << "Display set to line numbers." << std::endl;
            }
            else {
                std::cout << "Incorrect arguments. Correct format is: set_display <wrap/center> [width]" << std::endl;
                displayer = new DirectDisplayer();
            }
        }
    }
    else if (command == "history") {
        if (commandHistory.empty()) {
            std::cout << "Command history is empty." << std::endl;
        } else {
            std::cout << "--- Command History ---" << std::endl;
            // Обхождаме вектора и отпечатваме всяка команда с номер.
            for (size_t i = 0; i < commandHistory.size(); ++i) {
                std::cout << i + 1 << ": " << commandHistory[i] << std::endl;
            }
        }
    }
    // // Команда за отмяна на последната операция.
    else if (command == "undo") {
        if (undoStack.empty()) { std::cout << "Nothing to undo." << std::endl; }
        else {
            this->commandHistory.push_back("undo");
            // // 1. Взимаме указателя към последната команда от стека.
            Command* lastCommand = undoStack.back();
            // // 2. Премахваме указателя от стека.
            undoStack.pop_back();
            // // 3. Извикваме 'undo' метода на командата.
            lastCommand->undo();
            // // 4. Изтриваме обекта-команда. В тази имплементация няма 'redo' функционалност.
            redoStack.push_back(lastCommand);
            std::cout << "Undo successful." << std::endl;
        }
    }
    else if (command == "redo") {
        if (redoStack.empty()) { std::cout << "Nothing to redo." << std::endl; }
        else {
            // // 1. Взимаме указателя към последната команда от стека.
            Command* lastCommand = redoStack.back();
            // // 2. Премахваме указателя от стека.
            redoStack.pop_back();
            // // 3. Извикваме 'undo' метода на командата.
            lastCommand->execute();
            // // 4. Изтриваме обекта-команда. В тази имплементация няма 'redo' функционалност.
            undoStack.push_back(lastCommand);
            std::cout << "Redo successful." << std::endl;
        }
    }
}

// //
// // Основен метод, който стартира и поддържа изпълнението на програмата.
// // Той е входната точка и управлява главния цикъл на приложението (REPL - Read-Eval-Print Loop).
// //
void TextProcessor::run() {
    // // При стартиране, първо отпечатваме помощна информация за потребителя,
    // // която описва всички налични команди и техния синтаксис.
    std::cout << "C++ Text Processor\n\n"
              << "Available Commands: \n"
              << "  open <filename>          - Opens a file.\n"
              << "  save                     - Saves the active file.\n"
              << "  saveas <new_filename>    - Saves the active file under a new name.\n"
              << "  close                    - Closes the active file.\n"
              << "  switch <filename>        - Switches to an already open file.\n"
              << "  info                     - Shows information about all open files.\n"
              << "  on_block <from> <to> <command> - Runs a command on a specific block of lines.\n"
              << "  exit                     - Exits the application.\n"
              << "  replace <find> <replace> - Finds and replaces text.\n"
              << "  transform <toupper/tolower> - Changes text case.\n"
              << "  sort                     - Sorts lines.\n"
              << "  shuffle                  - Shuffles lines.\n"
              << "  unique                   - Removes duplicate lines.\n"
              << "  trim                     - Removes whitespace.\n"
              << "  count <word>             - Counts occurrences of a word.\n"
              << "  history                  - Shows the history of executed commands.\n"
              << "  redo                     - Redoes the last undone action.\n"
              << "  undo                     - Undoes the last action.\n"
              << "  edit <line> <text>       - Replaces the content of a line.\n"
              << "  insert <line> <text>     - Inserts a new line.\n"
              << "  remove <line>            - Removes a line.\n"
              << "  display                  - Prints the active file.\n"
              << "  set_display <direct/wrap/center/linenumbers> [width> - Changes the display mode.\n"
              << "  macro_begin <name>       - Starts recording a new macro.\n"
              << "  macro_end                - Stops recording.\n"
              << "  run_macro <name>         - Executes a saved macro.\n";

    // // Основният цикъл на приложението. Върти се, докато флагът 'isRunning' е 'true'.
    while (this->isRunning) {
        // // Отпечатваме промпт ("> "), за да покажем, че очакваме команда от потребителя.
        std::cout << "> ";
        std::string input;
        // // Спираме изпълнението и чакаме потребителят да въведе цял ред и да натисне Enter.
        std::getline(std::cin, input);

        // // Обработка на EOF (End-of-File) - сигнал, който се изпраща с Ctrl+D (Linux) или Ctrl+Z (Windows).
        // // Интерпретираме го като команда за изход за по-добро потребителско изживяване.
        if (std::cin.eof()) {
            input = "exit";
        }

        // // Делегираме обработката на въведения текст на метода processInput.
        // // Това е добър дизайн, който разделя отговорностите: run() управлява цикъла,
        // // а processInput() се занимава с логиката на командите.
        processInput(input);
    }

    // // Когато цикълът приключи (след команда 'exit'), отпечатваме финално съобщение.
    std::cout << "Exiting." << std::endl;
}

// //
// // Главен разпределител (dispatcher), който приема суров потребителски вход,
// // анализира го и решава коя част от програмата да го обработи.
// //
void TextProcessor::processInput(const std::string& input) {
    // // Игнорираме празен вход (напр. ако потребителят просто натисне Enter).
    if (input.empty()) return;

    // // Разделяме входа на две части: първата дума (командата) и всичко останало (аргументите).
    // // Например: "edit 5 new text" -> commandWord="edit", args="5 new text"
    std::string commandWord;
    std::string args;
    size_t firstSpace = input.find(' ');
    if (firstSpace == std::string::npos) { // // Ако няма интервал, целият вход е команда.
        commandWord = input;
        args = "";
    } else { // // Ако има интервал, разделяме входа на две.
        commandWord = input.substr(0, firstSpace);
        args = input.substr(firstSpace + 1);
    }

    // // Основна if-else if верига, която категоризира командите и делегира
    // // работата на съответните по-малки, специализирани методи.
    if (commandWord == "open" || commandWord == "save" || commandWord == "saveas" ||
        commandWord == "close" || commandWord == "switch" || commandWord == "info") {
        handleFileCommand(commandWord, args);
        }
    else if (commandWord == "macro_begin" || commandWord == "macro_end" || commandWord == "run_macro") {
        handleMacroCommand(commandWord, args);
    }
    else if (commandWord == "undo" || commandWord == "redo" || commandWord == "exit" || commandWord == "display" || commandWord == "set_display" || commandWord == "history") {
        handleAppCommand(commandWord, args);
    }
    else if (commandWord == "on_block") {
        if (!activeDocument) {
            std::cout << "Error: No active document." << std::endl;
            return;
        }

        // // 1. Парсираме аргументите (from, to, subCommandStr) - това остава същото.
        size_t from_end = args.find(' ');
        size_t to_end = args.find(' ', from_end + 1);
        if (from_end == std::string::npos || to_end == std::string::npos) {
            std::cout << "Incorrect arguments. Correct format is: on_block <from> <to> <command>" << std::endl;
            return;
        }
        size_t from = std::stoi(args.substr(0, from_end)) - 1;
        size_t to = std::stoi(args.substr(from_end + 1, to_end - from_end - 1)) - 1;
        std::string subCommandStr = args.substr(to_end + 1);

        // // Валидацията на обхвата също остава.
        size_t lineCount = activeDocument->getLineCount();
        if (from >= lineCount || to >= lineCount || from > to) {
            std::cout << "Error: Invalid range." << std::endl;
            return;
        }

        // // 2. СЪЗДАВАМЕ ПОД-КОМАНДАТА.
        // // Забележка: Тук я създаваме с activeDocument, но OnBlockCommand ще я пренасочи.
        // // По-сложен дизайн би създал временния документ тук и би го подал на парсера.
        Command* subCommand = parser->parse(subCommandStr, activeDocument);
        if (!subCommand) {
            std::cout << "Invalid command specified for 'on_block'." << std::endl;
            return;
        }

        // // 3. СЪЗДАВАМЕ НОВАТА "МЕТА-КОМАНДА" OnBlockCommand.
        Command* mainCommand = new OnBlockCommand(activeDocument, from, to, subCommand);

        // // 4. ПОДАВАМЕ Я НА СТАНДАРТНИЯ МЕХАНИЗЪМ ЗА ИЗПЪЛНЕНИЕ,
        // // който ще се погрижи за execute() и добавянето в undo стека.
        executeCommand(mainCommand);

        std::cout << "Block operation completed." << std::endl;
    }
    // // Ако командата не е от специалните, приемаме, че е стандартна,
    // // променяща съдържанието команда, която трябва да може да се отменя.
    else {
        if (activeDocument == nullptr) {
            std::cout << "Error: No active document. Use 'open <filename>'." << std::endl; return;
        }
        // // Извикваме парсера, за да създаде съответния обект-команда.
        Command* command = parser->parse(input, activeDocument);
        if (command != nullptr) { // // Ако парсерът е успял...
            // // ...подаваме създадената команда на executeCommand, който ще я изпълни
            // // и ще я добави в undo стека (или в записван макрос).
            this->commandHistory.push_back(input);
            executeCommand(command);
        } else { // // Ако парсерът върне nullptr...
            // // ...значи командата е непозната.
            std::cout << "Unknown command: '" << commandWord << "'" << std::endl;
        }
    }
}