#include <iostream>
#include <Windows.h>
#include <vector>
#include <conio.h>
#include <string>
#include <thread>
#define ANY_ARROW 224
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define UP_ARROW 72
#define DOWN_ARROW 80
using namespace std;

void hidecursor(bool boolean) {
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
    cci.bVisible = !boolean;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

void getcontentofdir(std::vector<string>& out, vector<bool> & isdir, const string& directory)
{
    //
    out.clear();
    isdir.clear();
    //
    HANDLE handle;
    WIN32_FIND_DATA wfd;
    wstring wstr = wstring(directory.begin(), directory.end());
    string ex = "/*";
    wstring ex1 = wstring(ex.begin(), ex.end());
    if ((handle = FindFirstFile((wstr + ex1).c_str(), &wfd)) == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        WCHAR * name = wfd.cFileName;
        wstring wsname(name);
        string sname(wsname.begin(), wsname.end());
        if (sname[0] != '.') {
            out.push_back(sname);
            isdir.push_back((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
        }
    } while (FindNextFile(handle, &wfd));
    FindClose(handle);
} // GetFilesInDirectory
int woc() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwSize.X;
}
int hoc() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwSize.Y;
}
void clear() { //may or may have not found this on stack overflow 😍😍
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    std::cout.flush();

    if (!GetConsoleScreenBufferInfo(console, &csbi)) {
        abort();
    }

    int uhoh = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD dw;
    COORD b = { 0,0 };
    FillConsoleOutputCharacter(console, TEXT(' '), uhoh, b, &dw);
    FillConsoleOutputAttribute(console, csbi.wAttributes, uhoh, b, &dw);
    SetConsoleCursorPosition(console, b);
}
void exfunc1(vector<string> nuhuh, int inc, string path, string start_path) {
    vector<string> yuhuh;
    if (path != start_path) {
        string expath = path;
        vector<bool> exv;
        expath = expath.substr(0, expath.find_last_of("\\", expath.find_last_of("\\") - 1) + 1);
        getcontentofdir(yuhuh, exv, expath);
    }
    clear();
    for (int i = 0; i < nuhuh.size(); i++) {
        if (inc == i) cout << "\033[30;47m";
        cout << nuhuh[i];
        if (inc == i) cout << "\033[0m";
        cout << "\n";
    }
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,(short)inc});
}
string finder(string path) {
    string start_path = path;
    hidecursor(true);
    int inc = 0;
    vector<string> nuhuh;
    vector<bool> dun;
    vector<int> prevInc;
    getcontentofdir(nuhuh, dun, path);
    exfunc1(nuhuh, inc, path, start_path);
    if (nuhuh.empty() == true) {
        return "finder(): invalid path! please try again with a different path.";
    }
    int i = 0;
    int j = 0;
    bool dirIsEmpty = false;
    bool ex1 = false;
    bool ex2 = false;
    while (i = _getch()) {
        switch (i) {
        case ANY_ARROW:
            i = _getch();
            switch (i) {
            case UP_ARROW:
                if (dirIsEmpty == false) {
                    if (inc > 0) {
                        inc--;
                        exfunc1(nuhuh, inc, path, start_path);
                    }
                }
                break;
            case DOWN_ARROW:
                if (dirIsEmpty == false) {
                    if (inc < nuhuh.size() - 1) {
                        inc++;
                        exfunc1(nuhuh, inc, path, start_path);
                    }
                }
                break;
            case LEFT_ARROW:
                if (path != start_path) { // "C:\\"
                    ex2 = ex1;
                    ex1 = false;
                    dirIsEmpty = false;
                    path = path.substr(0, path.find_last_of("\\", path.find_last_of("\\") - 1) + 1);
                    getcontentofdir(nuhuh, dun, path);
                    inc = prevInc.back();
                    prevInc.pop_back();
                    exfunc1(nuhuh, inc, path, start_path);
                }
                break;
            case RIGHT_ARROW:
                if (dirIsEmpty == false) {
                    if (dun[inc] == true) {
                        prevInc.push_back(inc);
                        path = path + nuhuh[inc] + "\\";
                        getcontentofdir(nuhuh, dun, path);
                        if (nuhuh.empty() == true) {
                            dirIsEmpty = true;
                        }
                        inc = 0;
                        exfunc1(nuhuh, inc, path, start_path);
                    }
                }
                break;
            }
            break;
        case 13:
            if (dirIsEmpty == false) {
                /*
                if (!(i == UP_ARROW || i == DOWN_ARROW || i == ANY_ARROW || i == LEFT_ARROW)) {
                    if (dun[inc] == true) {
                        path = path + nuhuh[inc] + "\\";
                        nuhuh = {};
                        getcontentofdir(nuhuh, dun, path);
                        inc = 0;
                        exfunc1(nuhuh, inc, path);
                    }
                }
                */
                hidecursor(false);
                path = path + nuhuh[inc] + "\\";
                return path;
            }
            break;
        }
        if (dirIsEmpty == true) {
            if (ex1 == false) {
                cout << "this folder is empty.";
                ex1 = true;
                ex2 = false;
                thread thread1([&ex1, &ex2]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
                    if (ex1 == true && ex2 == false) {
                        cout << "\n\nclick on the left arrow to go back.";
                    }
                    ex2 = false;
                    ex1 = false;
                });
                thread1.detach();
            }
        }
    }
}
void enableANSI()
{
    HANDLE ok = GetStdHandle(STD_OUTPUT_HANDLE);
    if (ok == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(ok, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(ok, dwMode);
}
int main()
{
    enableANSI();
    string path = finder("C:\\");
    clear();
    cout << path;

    while (!_kbhit()) {}
}
