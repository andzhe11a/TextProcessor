#pragma once
#include <vector>
#include <string>
#include "Displayer.h"

class TextDocument;
class Command;
class MacroCommand;
class CommandParser;

// // Централният клас на приложението - "мозъкът" или "двигателят".
// // Той управлява основния цикъл, всички отворени документи, историята за undo,
// // макросите и координира работата на всички останали компоненти.
class TextProcessor {
public:
    TextProcessor();
    ~TextProcessor();
    void run();
private:
    // Всички член-променливи, които пазят текущото състояние на текстоввия процесор.
    std::vector<TextDocument*> documents;     // Колекция от всички заредени в момента документи.
    TextDocument* activeDocument;             // Указател към документа, с който потребителят работи в момента.
    std::vector<Command*> undoStack;// Стек (реализиран с вектор) за историята на изпълнените команди, нужен за 'undo'.
    std::vector<Command*> redoStack;
    std::vector<std::string> commandHistory;
    Displayer* displayer;                     // Указател към текущата "стратегия" за извеждане на текст на екрана.
    bool isRunning;                           // Флаг, който контролира основния цикъл на програмата (докато е true, програмата работи).
    std::vector<MacroCommand*> macros;        // Колекция от всички дефинирани от потребителя макроси.
    MacroCommand* currentlyRecordingMacro;    // Указател, който е различен от nullptr само в периода на запис на макрос.
    CommandParser* parser;                    // Указател към обекта, който "превежда" потребителския вход в команди.

    void executeCommand(Command* command);           // Централизиран метод за изпълнение на команда и добавянето й в undo стека.
    void processInput(const std::string& input);     // Основен метод, който обработва един ред потребителски вход.
    void cleanup();                                  // Помощен метод, който се извиква от деструктора за освобождаване на паметта.
    void clearRedoStack();
    bool handleUnsavedChanges(TextDocument* doc);    // Логика за питане на потребителя дали да запази промените.

    // // Методи за обработка на различните категории команди.
    // // Разделянето на 'processInput' на по-малки методи прави кода по-подреден.
    void handleFileCommand(const std::string& command, const std::string& args); // Обработва команди като open, save, close...
    void handleMacroCommand(const std::string& command, const std::string& args); // Обработва команди, свързани с макроси.
    void handleAppCommand(const std::string& command, const std::string& args);   // Обработва команди за самото приложение (напр. exit, help).
};
