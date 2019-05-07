#include <napi.h>
#include "keypress.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    return winkey::Init(env, exports);
}

NODE_API_MODULE(winkey, InitAll)