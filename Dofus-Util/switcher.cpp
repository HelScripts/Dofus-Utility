#include "switcher.h"


static const int OPTION_1 = 49; // "1" key
static const int OPTION_2 = 50; // "2" key
static const int OPTION_3 = 51; // "3" key
static const int ENTER = 10; // "ENTER" key

static std::vector<std::pair<int, std::string>> binds{};
static std::vector<std::pair<int, std::string>> hotkeyNames{};

static std::vector<std::pair<int, int>> tempBinds{};

static int count = 1;

int main(){
    Switcher::setup();
}



void Switcher::setup() {
    int key = 0;
    while (key != OPTION_1) {

        if (key != ENTER) {
            std::cout << "Choose Option: " << std::endl;
            std::cout << "1: New Config" << std::endl;
        }

        std::cout << std::endl;
        key = std::cin.get();
        if (key != OPTION_1) {
            if (key == ENTER) continue;
            std::cout << "Error, please choose a valid option! " << "(" << key << ")" << std::endl << std::endl;
        }
    }

    do {
        setupAccount();

        std::cout << "Your current binds are: " << std::endl << std::endl;
        for (int i = 0; i < binds.size(); i++) {
            std::cout << hotkeyNames.at(i).second << ": " << binds.at(i).second << std::endl;
        }

        std::cout << std::endl << "Would you like to bind more accounts?" << std::endl;
        std::cout << "1: YES" << std::endl;
        std::cout << "2: NO" << std::endl;
        key = std::cin.get();
        while (key == ENTER) {
            key = std::cin.get();
        }
    } while (key != OPTION_2);

    std::cout << "Registering hotkeys..." << std::endl;
    for (auto temps : tempBinds) {
        RegisterHotKey(NULL, temps.first, 0, temps.second);
    }

    std::system("cls");
    std::cout << "Registered successfully! Please use the following binds, and enjoy!" << std::endl;
    for (int i = 0; i < binds.size(); i++) {
        std::cout << hotkeyNames.at(i).second << ": " << binds.at(i).second << std::endl;
    }
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            for (auto bind : binds) {
                if (bind.first == msg.wParam) {
                    std::string xd = bind.second + " - Dofus";
                    EnumWindows(Switcher::EnumWindowsProc, reinterpret_cast<LPARAM>(&xd));
                }
            }
        }

    }
}

void Switcher::setupAccount() {

    std::string name;
    int key = 0;
    int keybind = -1;
    char keyName[128] = "{UNNKNOWN}";
    do {
        std::cout << "Please input your character name (CaSe SeNsItIvE):" << std::endl;
        std::cin >> name;
        std::cout << std::endl << "Your character name is: " << name << ", is this correct?" << std::endl;
        std::cout << "1: YES" << std::endl;
        std::cout << "2: NO" << std::endl << std::endl;
        key = std::cin.get();
        while (key == ENTER) {
            key = std::cin.get();
        }
    } while (key != OPTION_1);


    do {
        keybind = -1;
        std::cout << "Please press a hotkey for this account... " << std::endl;
        bool start = true;
        while (keybind < 0) {
            for (int i = 0; i < 0xFE; i++) {
                if (i == VK_RETURN) continue;

                if (GetAsyncKeyState(i)) {
                    if (start) continue;
                    keybind = i;
                    key = i;
                    break;
                }
                else {
                    if (i == key) {
                        start = false;
                    }
                }
            }
        }
        GetKeyNameTextA(MapVirtualKeyA(keybind, MAPVK_VK_TO_VSC) << 16, keyName, sizeof(keyName) / sizeof(char));
        std::cout << std::endl << "Your chosen keybind is " << keyName << ", is this correct?" << std::endl;
        std::cout << "1: YES" << std::endl;
        std::cout << "2: NO" << std::endl << std::endl;

        key = std::cin.get();
        while (key == ENTER) {
            key = std::cin.get();
        }

    } while (key != OPTION_1);

    std::cout << "Bound account " << name << " to key: " << keyName << std::endl << std::endl;
    tempBinds.emplace_back(count, keybind);
    binds.emplace_back(count, name);
    hotkeyNames.emplace_back(count, keyName);
    count++;
}