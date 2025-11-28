// vim: set sw=4 ts=4 ft=cpp et fenc=utf-8:
#include <windows.h>
#include <wlanapi.h>
#include "wlanapi_supplement.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>

// Constants
#define IDC_MAIN_BUTTON 101
#define IDC_SSID_EDIT 102
#define IDC_PASSWORD_EDIT 103

// Forward declarations
void HandleLegacyHotspot(HWND hwnd);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern "C" int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow);

void HandleLegacyHotspot(HWND hwnd) {
    HANDLE hClient = NULL;
    DWORD dwMaxClient = 2;
    DWORD dwCurVersion = 0;
    DWORD dwResult = 0;

    dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
    if (dwResult != ERROR_SUCCESS) {
        MessageBox(hwnd, L"WlanOpenHandle failed", L"Error", MB_OK);
        return;
    }

    int ssid_len_w = GetWindowTextLengthW(GetDlgItem(hwnd, IDC_SSID_EDIT)) + 1;
    std::vector<wchar_t> ssid_w(ssid_len_w);
    GetDlgItemTextW(hwnd, IDC_SSID_EDIT, ssid_w.data(), ssid_len_w);
    int ssid_len = WideCharToMultiByte(CP_ACP, 0, ssid_w.data(), -1, NULL, 0, NULL, NULL);
    std::vector<char> ssid_buf(ssid_len);
    WideCharToMultiByte(CP_ACP, 0, ssid_w.data(), -1, ssid_buf.data(), ssid_len, NULL, NULL);
    std::string ssidStr(ssid_buf.data());

    if (ssidStr.length() < 1 || ssidStr.length() > 32) {
        MessageBox(hwnd, L"SSID must be between 1 and 32 characters.", L"Error", MB_OK);
        return;
    }

    WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS settings;
    settings.dwMaxNumberOfPeers = 10;
    settings.hostedNetworkSSID.uSSIDLength = ssidStr.length();
    for (size_t i = 0; i < ssidStr.length(); ++i) {
        settings.hostedNetworkSSID.ucSSID[i] = (UCHAR)ssidStr[i];
    }

    dwResult = WlanHostedNetworkSetProperty(
        hClient,
        wlan_hosted_network_opcode_connection_settings,
        sizeof(WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS),
        &settings,
        NULL,
        NULL
    );
    if (dwResult != ERROR_SUCCESS) {
        MessageBox(hwnd, L"WlanHostedNetworkSetProperty (SSID) failed", L"Error", MB_OK);
        WlanCloseHandle(hClient, NULL);
        return;
    }

    int key_len_w = GetWindowTextLengthW(GetDlgItem(hwnd, IDC_PASSWORD_EDIT)) + 1;
    std::vector<wchar_t> key_w(key_len_w);
    GetDlgItemTextW(hwnd, IDC_PASSWORD_EDIT, key_w.data(), key_len_w);
    int key_len = WideCharToMultiByte(CP_ACP, 0, key_w.data(), -1, NULL, 0, NULL, NULL);
    std::vector<char> key_buf(key_len);
    WideCharToMultiByte(CP_ACP, 0, key_w.data(), -1, key_buf.data(), key_len, NULL, NULL);
    std::string keyStr(key_buf.data());

    if (keyStr.length() < 8 || keyStr.length() > 63) {
        MessageBox(hwnd, L"Password must be between 8 and 63 characters.", L"Error", MB_OK);
        return;
    }

    dwResult = WlanHostedNetworkSetSecondaryKey(
        hClient,
        keyStr.length(),
        (PUCHAR)keyStr.c_str(),
        TRUE,
        FALSE,
        NULL,
        NULL
    );
    if (dwResult != ERROR_SUCCESS) {
        MessageBox(hwnd, L"WlanHostedNetworkSetSecondaryKey failed", L"Error", MB_OK);
        WlanCloseHandle(hClient, NULL);
        return;
    }

    dwResult = WlanHostedNetworkStartUsing(hClient, NULL, NULL);
    if (dwResult != ERROR_SUCCESS) {
        MessageBox(hwnd, L"WlanHostedNetworkStartUsing failed", L"Error", MB_OK);
    } else {
        MessageBox(hwnd, L"Legacy hotspot started successfully.", L"Success", MB_OK);
    }

    WlanCloseHandle(hClient, NULL);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_MAIN_BUTTON) {
                HandleLegacyHotspot(hwnd);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

extern "C" int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"APModeWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"AP Mode",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 350, 200,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    CreateWindowW(L"STATIC", L"SSID:", WS_VISIBLE | WS_CHILD, 10, 10, 50, 20, hwnd, NULL, NULL, NULL);
    CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 70, 10, 250, 20, hwnd, (HMENU)IDC_SSID_EDIT, NULL, NULL);

    CreateWindowW(L"STATIC", L"Password:", WS_VISIBLE | WS_CHILD, 10, 40, 70, 20, hwnd, NULL, NULL, NULL);
    CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 70, 40, 250, 20, hwnd, (HMENU)IDC_PASSWORD_EDIT, NULL, NULL);

    CreateWindowW(
        L"BUTTON",
        L"Enable Hotspot",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        100, 100, 150, 40,
        hwnd,
        (HMENU)IDC_MAIN_BUTTON,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
