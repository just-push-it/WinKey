#include <napi.h>
#define WINVER 0x0500
#include <windows.h>

namespace winkey {
    typedef unsigned short ushort32_t;

    void pressKeys(const std::string &keys);
    void PressKeysWrapped(const Napi::CallbackInfo& info);

    Napi::Object Init(Napi::Env env, Napi::Object exports);
    unsigned short getKeyCodeFromString(std::string keyCode);
}