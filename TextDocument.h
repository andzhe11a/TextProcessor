#pragma once
#include <string>
#include <vector>
#include "BaseLine.h"

// Клас, който представя един текстов файл, зареден в паметта.
// Управлява колекцията от редове, състоянието на документа (незапазени промени)
// и операциите по четене от и запис на диска.
class TextDocument {
public:
    // Конструктор, който създава документ от файлов път
    TextDocument(const std::string& path);

    ~TextDocument();

    // --- Файлови операции ---
    // Методи за зареждане на съдържанието от диска, запис на текущите промени
    // и запис на съдържанието в нов файл.
    bool load();
    bool save();
    bool saveAs(const std::string& newPath);

    // --- Методи за информация и състояние ---
    // Предоставят информация "само за четене" за името на файла,
    // дали има незапазени промени, както и броя редове и символи.
    std::string getFilename() const;
    bool hasUnsavedChanges() const;
    size_t getLineCount() const;
    size_t getCharacterCount() const;

    // --- Методи за редакция на съдържанието ---
    // Осигуряват интерфейс за достъп и манипулация на редовете в документа.
    BaseLine* getLineObject(size_t index) const; // Връща указател към самия Line обект.
    std::string getLineText(size_t index) const; // Връща само текста на реда.
    void setLineText(size_t index, const std::string& newText); // Заменя съществуващ ред.
    void insertLine(size_t index, const std::string& newText); // Вмъква нов ред.
    void removeLine(size_t index); // Премахва съществуващ ред.

private:
    // Пътят до файла на диска, с който е свързан този документ.
    std::string filename;

    // Флаг, който се вдига на 'true' при всяка промяна на съдържанието.
    bool unsavedChanges;

    // Колекция от указатели към редовете на документа. Използваме указатели,
    // за да можем да работим полиморфно с различните типове редове (Numbered, Quoted и т.н.).
    std::vector<BaseLine*> lines;

    // Помощен метод за освобождаване на паметта, заета от всички Line обекти.
    // Извиква се от деструктора и при презареждане на файла.
    void clear();
};