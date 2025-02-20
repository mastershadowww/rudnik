#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>

using namespace std;

struct KeyEvent {
    int key;
    bool pressed;
    DWORD time;
};

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
        while (GetTickCount() - startTime < event.time) {
            Sleep(1);
        }

        INPUT input = { 0 };
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = event.key;
        input.ki.dwFlags = event.pressed ? 0 : KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));

        cout << "Taster: " << event.key << (event.pressed ? " pritisnut" : " pušten") << endl;
    }
}

int main() {
    cout << "Pritisnite ENTER za pokretanje snimljenih tastera..." << endl;
    cin.get();
    playKeys();
    return 0;
}
