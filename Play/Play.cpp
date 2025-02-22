#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>

using namespace std;

struct KeyEvent {
    int key;
    bool pressed;
    DWORD time;
};

void typeText(const string& text) {
    for (char c : text) {
        SHORT vk = VkKeyScan(c);
        keybd_event(vk, 0, 0, 0);
        keybd_event(vk, 0, KEYEVENTF_KEYUP, 0);
        Sleep(50);
    }
}

void pressEnter() {
    keybd_event(VK_RETURN, 0, 0, 0);
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}

bool getCode(string& code) {
    FILE* pipe = _popen("python C:\\Users\\lazar\\Desktop\\shadowww\\rudnik\\screenshot\\screenshot.py", "r");
    if (!pipe) return false;

    char buffer[128];
    string result = "";

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }
    _pclose(pipe);

    if (result.find("None") != string::npos) {
        return false;
    }

    code = result.substr(0, 3);
    return true;
}

void moveMouseDown() {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dx = 0;
    input.mi.dy = 50;
    input.mi.mouseData = 0;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.time = 0;
    input.mi.dwExtraInfo = 0;

    for (int i = 0; i < 3000; i += 50)
    {
        SendInput(1, &input, sizeof(INPUT));
        Sleep(5);
    }
}

void playKeys() {
    vector<KeyEvent> loadedKeys;
    ifstream file("keys.log", ios::binary);

    if (!file.is_open()) {
        cerr << "Nije pronađen snimak tastera!" << endl;
        return;
    }

    file.seekg(0, ios::end);
    size_t size = file.tellg() / sizeof(KeyEvent);
    file.seekg(0, ios::beg);
    loadedKeys.resize(size);
    file.read(reinterpret_cast<char*>(loadedKeys.data()), size * sizeof(KeyEvent));
    file.close();

    cout << "Pokretanje snimljenih tastera..." << endl;
    DWORD startTime = GetTickCount();
    for (const auto& event : loadedKeys) {
        while (GetTickCount() - startTime < event.time)
            Sleep(1);

        if (event.key == VK_MULTIPLY && event.pressed) {
            moveMouseDown();
        }
        else {
            INPUT input = { 0 };
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = event.key;
            input.ki.dwFlags = event.pressed ? 0 : KEYEVENTF_KEYUP;
            SendInput(1, &input, sizeof(INPUT));

            cout << "Taster: " << event.key << (event.pressed ? " pritisnut" : " pušten") << endl;
        }
    }
}

int main() {
    int cycleCount = 0;
    int failCount = 0;

    while (true) {
        string code;

        while (true) {
            typeText("t/rudnik");
            pressEnter();
            Sleep(2000);

            if (getCode(code)) {
                failCount = 0;
                typeText(code);
                pressEnter();
                cout << "Unet kod: " << code << endl;
                break;
            }
            else {
                failCount++;
                cout << "Kod nije pronadjen, ponavljamo pokusaj..." << endl;
                if (failCount >= 6) {
                    cout << "Previše neuspelih pokušaja, prekidam skriptu..." << endl;
                    return 1;
                }
                Sleep(2000);
            }
        }

        Sleep(2000);
        playKeys();

        cycleCount++;
        cout << "Zavrsen ciklus broj: " << cycleCount << endl;

        if (cycleCount % 10 == 0) {
            cout << "Izvrsavam komandu t/sendvic nakon 10 ciklusa..." << endl;
            typeText("t/sendvic");
            pressEnter();
            Sleep(2000);
        }

        cout << "Započinjem novi ciklus..." << endl;
        Sleep(2000);
    }

    return 0;
}
