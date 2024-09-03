#include <windows.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>

using namespace std;

void printWindowInfo(HWND hwnd, ofstream& logFile) {
    if (!hwnd) return;

    char class_name[100];
    char title[256];
    DWORD pid;
    RECT rect;
    SYSTEMTIME lt;

    // 获取系统本地时间
    GetLocalTime(&lt);

    // 获取窗口类名和标题
    GetClassNameA(hwnd, class_name, sizeof(class_name));
    GetWindowTextA(hwnd, title, sizeof(title));
    GetWindowRect(hwnd, &rect);
    const DWORD tid = GetWindowThreadProcessId(hwnd, &pid);
    char buffer[10240];
    sprintf(buffer, "[%02d:%02d:%02d.%03d]: PID=%ld, TID=%ld, HWND=%llu, Rect=(%d, %d, %d, %d), ClassName=%s, Title=%s",
            lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds,
            pid, tid,
            reinterpret_cast<uintptr_t>(hwnd),
            rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, class_name, title);
    cout << buffer << endl;
    logFile << buffer << endl;
}


int main() {
    ofstream logFile("log.txt", ios::app);
    if (!logFile.is_open()) {
        cerr << "无法打开日志文件！" << endl;
        return -1;
    }
    HWND lastFocus = nullptr;
    while (true) {
        const HWND focus = GetForegroundWindow();
        if (focus != lastFocus) {
            printWindowInfo(focus, logFile);
            lastFocus = focus;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
