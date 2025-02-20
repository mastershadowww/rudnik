// Record.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <thread>
#include <map>

using namespace std;

struct KeyEvent
{
    int key;
    bool pressed;
    DWORD time;
};

vector<KeyEvent> recordedKeys;
bool recording = true;
map<int, bool> keyStates;

void recordKeys()
{
    DWORD startTime = GetTickCount();
    while (recording)
    {
        for (int key = 8; key <= 190; key++)
        {
            bool isPressed = GetAsyncKeyState(key) & 0x8000;
            if (isPressed != keyStates[key])
            {
                recordedKeys.push_back({ key, isPressed, GetTickCount() - startTime });
                keyStates[key] = isPressed;
                cout << "Snimljen taster: " << key << (isPressed ? " pritisnut" : " pušten") << endl;
            }
        }
        Sleep(1);
    }

    ofstream file("keys.log", ios::binary);
    file.write(reinterpret_cast<char*>(recordedKeys.data()), recordedKeys.size() * sizeof(KeyEvent));
    file.close();
}

void playKeys()
{
    vector<KeyEvent> loadedKeys;
    ifstream file("keys.log", ios::binary);

    if (!file.is_open())
    {
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
    DWORD lastTime = GetTickCount();
    for (const auto& event : loadedKeys)
    {
        DWORD currentTime = GetTickCount();
        Sleep(event.time - (currentTime - lastTime));
        lastTime = GetTickCount();

        keybd_event(event.key, 0, event.pressed ? 0 : KEYEVENTF_KEYUP, 0);
        cout << "Taster: " << event.key << (event.pressed ? " pritisnut" : " pušten") << endl;
    }
}

int main()
{
    cout << "Pritisnite ENTER da počnete snimanje..." << endl;
    cin.get();
    thread recorder(recordKeys);
    cout << "Snimanje u toku, pritisnite ENTER da zaustavite..." << endl;
    cin.get();
    recording = false;
    recorder.join();

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
