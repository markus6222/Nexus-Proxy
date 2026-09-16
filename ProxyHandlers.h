
std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* current_time = std::localtime(&now_c);

    std::stringstream timeStream;
    timeStream << std::put_time(current_time, "%T");
    return timeStream.str();
}

//  ----------------- HOTKEYS  -----------------  //

bool keyPressed = false;
HHOOK keyboardHook = nullptr;

LRESULT CALLBACK LowLevelKeyboardProcFor1(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* kbdStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        char pressedKey = static_cast<char>(kbdStruct->vkCode);

        //std::cout << "Pressed Key: " << pressedKey << std::endl;

        globalhotkey1 = pressedKey;
        keyPressed = true;

        UnhookWindowsHookEx(keyboardHook);
        PostQuitMessage(0);
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelKeyboardProcFor2(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* kbdStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        char pressedKey = static_cast<char>(kbdStruct->vkCode);

        //std::cout << "Pressed Key: " << pressedKey << std::endl;

        globalhotkey2 = pressedKey;
        keyPressed = true;

        UnhookWindowsHookEx(keyboardHook);
        PostQuitMessage(0);
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelKeyboardProcFor3(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* kbdStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        char pressedKey = static_cast<char>(kbdStruct->vkCode);

        //std::cout << "Pressed Key: " << pressedKey << std::endl;

        globalhotkey3 = pressedKey;
        keyPressed = true;

        UnhookWindowsHookEx(keyboardHook);
        PostQuitMessage(0);
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelKeyboardProcFor4(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* kbdStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        char pressedKey = static_cast<char>(kbdStruct->vkCode);

        //std::cout << "Pressed Key: " << pressedKey << std::endl;

        globalhotkey4 = pressedKey;
        keyPressed = true;

        UnhookWindowsHookEx(keyboardHook);
        PostQuitMessage(0);
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelKeyboardProcFor5(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* kbdStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        char pressedKey = static_cast<char>(kbdStruct->vkCode);

        //std::cout << "Pressed Key: " << pressedKey << std::endl;

        globalhotkey5 = pressedKey;
        keyPressed = true;

        UnhookWindowsHookEx(keyboardHook);
        PostQuitMessage(0);
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

//  ----------------- HOTKEYS  LAST  -----------------  //



//  ----------------- STRING REVOCER  -----------------  //

std::string duzeltilmisString(const std::string& bozukString) {
    std::string duzeltilmis;

    for (char ch : bozukString) {
        if (std::isalnum(ch) || ch == '/') {
            duzeltilmis += ch;
        }
    }

    return duzeltilmis;
}

std::string duzeltilmisStringForFond(const std::string& bozukString) {
    std::string duzeltilmis;

    for (char ch : bozukString) {
        if (std::isalnum(ch) || ch == '/' || ch == ' ' || (duzeltilmis.empty() && std::isupper(ch))) {
            duzeltilmis += ch;
        }
    }

    return duzeltilmis;
}

//  ----------------- STRING REVOCER LAST  -----------------  //



//  ----------------- CALCULATOR  -----------------  //
int getPriority(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

double applyOperation(double a, double b, char op) {
    switch (op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b != 0)
            return a / b;
        else {
            cerr << "Hata: 0'a bölme hatasý!" << endl;
            exit(1);
        }
    default:
        cerr << "Hata: Geçersiz operatör!" << endl;
        exit(1);
    }
}

double evaluateExpression(const string& expression) {
    stack<double> values;
    stack<char> operators;

    istringstream iss(expression);
    string token;

    while (iss >> token) {
        size_t pos = 0;
        while ((pos = token.find_first_of("+-*/", pos)) != string::npos) {
            token.insert(pos, " ");
            token.insert(pos + 2, " ");
            pos += 3;
        }
    }

    istringstream iss2(token);
    while (iss2 >> token) {
        if (isdigit(token[0])) {
            double value = stod(token);
            values.push(value);
        }
        else if (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/') {
            char op = token[0];

            while (!operators.empty() && getPriority(operators.top()) >= getPriority(op)) {
                double b = values.top();
                values.pop();

                double a = values.top();
                values.pop();

                values.push(applyOperation(a, b, operators.top()));
                operators.pop();
            }

            operators.push(op);
        }
        else {
            cerr << "Hata: Geçersiz karakter!" << endl;
            exit(1);
        }
    }

    while (!operators.empty()) {
        double b = values.top();
        values.pop();

        double a = values.top();
        values.pop();

        values.push(applyOperation(a, b, operators.top()));
        operators.pop();
    }

    return values.top();
}

//  ----------------- CALCULATOR LAST  -----------------  //

//  ----------------- FPS  -----------------  //
int LoadPrivilege(void) {
    HANDLE hToken;
    LUID Value;
    TOKEN_PRIVILEGES tp;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return(GetLastError());
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Value))
        return(GetLastError());
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = Value;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL))
        return(GetLastError());
    CloseHandle(hToken);
    return 1;
}

DWORD GetProcessIdx(const char* processName)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Hata" << std::endl;
        return 0;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe))
    {
        std::cerr << "Hata" << std::endl;
        CloseHandle(hSnapshot);
        return 0;
    }

    while (Process32Next(hSnapshot, &pe))
    {
        if (strcmp(processName, pe.szExeFile) == 0)
        {
            CloseHandle(hSnapshot);
            return pe.th32ProcessID;
        }
    }

    CloseHandle(hSnapshot);
    return 0;
}

DWORD64 GetModuleBaseAddress(DWORD procId, const char* modName) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry)) {
            do {
                if (!_stricmp(modEntry.szModule, modName)) {
                    CloseHandle(hSnap);
                    return (DWORD64)modEntry.modBaseAddr;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return 0;
}
//  ----------------- FPS LAST -----------------  //