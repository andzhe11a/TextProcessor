cmake_minimum_required(VERSION 3.31)
project(TextProcessor)

set(CMAKE_CXX_STANDARD 20)

add_executable(TextProcessor main.cpp
        Command.h
        Displayer.h
        TextDocument.cpp
        TextDocument.h
        TextProcessor.cpp
        TextProcessor.h
        BaseLine.cpp
        BaseLine.h
        LineTypes.cpp
        LineTypes.h
        SetLineTextCommand.cpp
        SetLineTextCommand.h
        InsertLineCommand.cpp
        InsertLineCommand.h
        RemoveLineCommand.cpp
        RemoveLineCommand.h
        TransformCommand.cpp
        TransformCommand.h
        ReplaceCommand.cpp
        ReplaceCommand.h
        SortCommand.cpp
        SortCommand.h
        ShuffleCommand.cpp
        ShuffleCommand.h
        UniqueCommand.cpp
        UniqueCommand.h
        TrimCommand.cpp
        TrimCommand.h
        MacroCommand.cpp
        MacroCommand.h
        DirectDisplayer.cpp
        DirectDisplayer.h
        WrapDisplayer.cpp
        WrapDisplayer.h
        CenterDisplayer.cpp
        CenterDisplayer.h
        CommandParser.cpp
        CommandParser.h
        OnBlockCommand.h
        OnBlockCommand.cpp
)
