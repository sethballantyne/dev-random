// CommandParser.cpp : main project file.

#include "stdafx.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Text;

delegate void Handler();



public ref class Test
{
    int stringPosition = 0;
    String ^input;
    Queue<String^>^ parsedStrings = gcnew Queue<String^>();
    Dictionary<String ^, Handler ^>^ commands = gcnew Dictionary<String ^, Handler ^>();

    void PrintHandler()
    {
        try
        {
            Console::WriteLine(parsedStrings->Dequeue());
        }
        catch(...)
        {
            Console::WriteLine("no argument supplied.");
        }
    }
public:
    Test()
    {
        commands->Add("print", gcnew Handler(this, &Test::PrintHandler));
    }
    String ^ReadText(char delimiter)
    {
        StringBuilder^ temp = gcnew StringBuilder();
        while(stringPosition != input->Length)
        {
            if(input[stringPosition] != delimiter)
            {
                temp->Append(input[stringPosition]);
            }

            if((stringPosition + 1 >= input->Length) || (input[stringPosition] == delimiter))
            {
                return temp->ToString();
            }

            stringPosition++;
        }
    }

    void ParseString(String ^input)
    {
        this->input = input;

        while(stringPosition != input->Length)
        {
            String ^parsedText;
            if(input[stringPosition] == '"')
            {
                // parsing a string.
                // advance the index so it's pointing to the first character in the string
                // and request the string be parsed.
                stringPosition++;
                parsedText = ReadText('"');

            }
            else
            {
                parsedText = ReadText(' ');
            }

            if(parsedText != String::Empty)
            {
                parsedStrings->Enqueue(parsedText);
                parsedText = String::Empty;
            }

            stringPosition++;
        }

        if(parsedStrings->Count > 0)
        {
            String^ command;
            try
            {
                command = parsedStrings->Dequeue();
                commands[command]->Invoke();
            }
            catch(...)
            {
                Console::WriteLine("Invalid command: {0}", command);
            }
        }
        
    }
};

int main(array<System::String ^> ^args)
{
    String^ input = nullptr;

    while(input != "quit")
    {
        input = Console::ReadLine();

        Test^ t = gcnew Test();
        t->ParseString(input);
    }

    //func(gcnew Handler(func2));
    return 0;
}
