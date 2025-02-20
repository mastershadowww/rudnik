#include <windows.h>
#include <iostream>

int main() {
    // Čekaj 5 sekundi pre nego što počneš sa simulacijom
    Sleep(5000); // 5000 ms = 5 sekundi

    // Virtuelni kod za taster 'W'
    BYTE vk_W = 0x57; // 0x57 je tačan kod za taster 'W'

    // Simulacija pritiska na 'W' taster
    keybd_event(vk_W, 0, 0, 0); // Pritisni taster 'W'
    std::cout << "Pritisnut je taster W" << std::endl;
    Sleep(5000); // Drži taster W 5 sekundi

    // Otpusti taster 'W'
    keybd_event(vk_W, 0, KEYEVENTF_KEYUP, 0);
    std::cout << "Otpusten je taster W" << std::endl;

    return 0;
}
