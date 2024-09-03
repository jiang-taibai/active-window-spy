#include <windows.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>

using namespace std;

const int& CLASS_NAME_MAX_LENGTH = 1024;
const int& TITLE_MAX_LENGTH = 1024;
const int& BUFFER_MAX_LENGTH = 10240;
const string& LOG_FILE_NAME = "log.txt";
const int& SLEEP_TIME = 100;

ofstream openLogFile(const string &filename) {
    std::ofstream logFile(filename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Unable to open log file!" << std::endl;
    }
    return logFile;
}

void disableQuickEdit() {
    HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(hConsole, &consoleMode);
    consoleMode &= ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(hConsole, consoleMode);
}

void printWindowInfo(HWND hwnd, ofstream &logFile) {
    if (!hwnd) return;

    char class_name[CLASS_NAME_MAX_LENGTH];
    char title[TITLE_MAX_LENGTH];
    DWORD pid;
    RECT rect;
    SYSTEMTIME lt;
    GetLocalTime(&lt);
    GetClassNameA(hwnd, class_name, sizeof(class_name));
    GetWindowTextA(hwnd, title, sizeof(title));
    GetWindowRect(hwnd, &rect);
    const DWORD tid = GetWindowThreadProcessId(hwnd, &pid);

    char buffer[BUFFER_MAX_LENGTH];
    sprintf(buffer, "[%02d:%02d:%02d.%03d]: PID=%ld, TID=%ld, HWND=%llu, Rect=(%d, %d, %d, %d), ClassName=%s, Title=%s",
            lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds,
            pid, tid,
            reinterpret_cast<uintptr_t>(hwnd),
            rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, class_name, title);
    cout << buffer << endl;
    if (logFile.is_open()) logFile << buffer << endl;
}

[[noreturn]] int main() {
    ofstream logFile = openLogFile(LOG_FILE_NAME);
    HWND lastFocus = nullptr;
    HWND focus = nullptr;
    while (true) {
        focus = GetForegroundWindow();
        if (focus != lastFocus) {
            printWindowInfo(focus, logFile);
            lastFocus = focus;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
    }
}
