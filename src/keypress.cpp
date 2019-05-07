#include "keypress.h"
#include <map>
#include <WinUser.h>

struct SpecialKeys {
    static std::map<std::string, unsigned short> create_map() {
        std::map<std::string, unsigned short> m;
        m["@@enter"] = VK_RETURN;
        m["@@return"] = VK_RETURN;
        m["@@back"] = VK_BACK;
        m["@@backspace"] = VK_BACK;
        m["@@control"] = VK_CONTROL;
        m["@@ctrl"] = VK_CONTROL;
        m["@@space"] = VK_SPACE;
        m["@@delete"] = VK_DELETE;
        m["@@tab"] = VK_TAB;
        return m;
    }
};
const std::map<std::string, unsigned short> sKeys = SpecialKeys::create_map();

void sendSpecialKey(const unsigned short &code) {
    INPUT in[2];
    in[0].type = INPUT_KEYBOARD;
    in[0].ki.wVk = code;

    in[1] = in[0];
    in[1].ki.dwFlags |= KEYEVENTF_KEYUP;
    
    SendInput(2, in, sizeof(INPUT));
}

void winkey::pressKeys(const std::string &keys) {
    const int len = keys.length();
    if(len == 0) return;

    if(sKeys.find(keys) != sKeys.end()) {
         sendSpecialKey(sKeys.at(keys));
         return;
    }

    std::vector<INPUT> in(len*2);
    // Zero out memory on inputs
    ZeroMemory(&in[0], in.size()*sizeof(INPUT));

    int i = 0, idx = 0;
    while(i < len) {
        unsigned short ch = (unsigned short) keys[i++];

        // This range is apparently not useable for UTF16, but is for UTF8
        if((ch < 0xD800) || (ch > 0xDFFF)) {
            in[idx].type = INPUT_KEYBOARD;
            in[idx].ki.wScan = ch;
            in[idx].ki.dwFlags = KEYEVENTF_UNICODE;
            ++idx;

            in[idx] = in[idx-1];
            in[idx].ki.dwFlags |= KEYEVENTF_KEYUP;
        } else {
            in[idx].type = INPUT_KEYBOARD;
            in[idx].ki.wScan = ch;
            in[idx].ki.dwFlags = KEYEVENTF_UNICODE;
            ++idx;

            in[idx].type = INPUT_KEYBOARD;
            in[idx].ki.wScan = (unsigned short) keys[i++];
            in[idx].ki.dwFlags = KEYEVENTF_UNICODE;
            ++idx;

            in[idx] = in[idx-2];
            in[idx].ki.dwFlags |= KEYEVENTF_KEYUP;
            ++idx;

            in[idx] = in[idx-2];
            in[idx].ki.dwFlags |= KEYEVENTF_KEYUP;
            ++idx;
        }
    }

    SendInput(in.size(), in.data(), sizeof(INPUT));
}

void winkey::PressKeysWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if(info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }

    Napi::String keys = info[0].As<Napi::String>();

    pressKeys(keys.Utf8Value());
}

Napi::Object winkey::Init(Napi::Env env, Napi::Object exports) {
    exports.Set("pressKeys", Napi::Function::New(env, winkey::PressKeysWrapped));

    return exports;
}
