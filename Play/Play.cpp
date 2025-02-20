// Play.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
