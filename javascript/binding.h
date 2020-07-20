#include <napi.h>
#include <string>

extern "C" {
  #include "../src/buffer.h"
  #include "../src/memory.h"
  #include "../src/node.h"
  #include "../src/options.h"
  #include "../src/print.h"
}

namespace binding {
  Napi::String Print(const Napi::CallbackInfo& info);
  Napi::Object Init(Napi::Env env, Napi::Object exports);
}
